#version 140

#define M_PI                (3.1415926535897932384626433832795)

uniform vec3 Resolution;
uniform float Time;
uniform mat4 ViewMatrix;

in vec2 FragCoord;
out vec4 FragColor;

#define MIN_DELTA       (0.001 / Resolution.y)
#define MAX_DELTA       (0.1 / Resolution.y)
#define MAX_DISTANCE    (48.0)

#define MAX_ITERATIONS  (64)

struct surface
{
    int object;
    int iteration;
    float orbit;
};

// ----------------------------------------------------------------------------
// I don't know where these came from. But thanks to whoever wrote these.
vec3 vRotateX(vec3 p, float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return vec3(p.x, c*p.y+s*p.z, -s*p.y+c*p.z);
}

vec3 vRotateY(vec3 p, float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return vec3(c*p.x-s*p.z, p.y, s*p.x+c*p.z);
}

vec3 vRotateZ(vec3 p, float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return vec3(c*p.x+s*p.y, -s*p.x+c*p.y, p.z);
}

// ----------------------------------------------------------------------------
// Kaleidoscopic (escape time) IFS
// knighty
// http://www.fractalforums.com/sierpinski-gasket/kaleidoscopic-(escape-time-ifs)/
#define FRACT_ITER      (22)
#define FRACT_SCALE     (1.8)
#define FRACT_OFFSET    (1.0)
float DE(vec3 z)
{
    float c = 2.0;
    //z.y = mod(z.y, c)-c/2.0;

    //z = vRotateZ(z, M_PI/2.0);

    float r;
    int n1 = 0;
    for (int n = 0; n < FRACT_ITER; n++) {
        float rotate = M_PI*0.5; // + 0.1 * sin(Time / 1000.0);
        z = vRotateX(z, rotate);
        z = vRotateY(z, rotate);
        z = vRotateZ(z, rotate);

        z.xy = abs(z.xy);
        if (z.x+z.y<0.0) z.xy = -z.yx; // fold 1
        if (z.x+z.z<0.0) z.xz = -z.zx; // fold 2
        if (z.y+z.z<0.0) z.zy = -z.yz; // fold 3
        z = z*FRACT_SCALE - FRACT_OFFSET*(FRACT_SCALE-1.0);
    }
    return (length(z) ) * pow(FRACT_SCALE, -float(FRACT_ITER));
}

// ------------------------------------------------------------------------------------------------
#define DE3_ITER            (32)
#define DE3_SCALE           (2.0f)
#define DE3_MIN_RADIUS      (0.5f)
#define DE3_FIXED_RADIUS    (1.0f)
#define DE3_FOLDING_LIMIT   (1.0f)
void sphereFold(inout vec3 z, inout float dz)
{
    float r2 = dot(z, z);

    if (r2 < DE3_MIN_RADIUS)
    { 
        const float temp = DE3_FIXED_RADIUS / DE3_MIN_RADIUS;
        z *= temp;
        dz *= temp;
    }
    else if (r2 < DE3_FIXED_RADIUS)
    { 
        float temp = DE3_FIXED_RADIUS / r2;
        z *= temp;
        dz *= temp;
    }
}

void boxFold(inout vec3 z)
{
    const vec3 foldingLimit = vec3(DE3_FOLDING_LIMIT, DE3_FOLDING_LIMIT, DE3_FOLDING_LIMIT);
    z = clamp(z, -foldingLimit, foldingLimit) * 2.0f - z;
}

float DE3(vec3 z, out float orbit)
{
    vec3 offset = z;
    float dr = 1.0f;

    float min_dist = -1e9f;

    for (int n = 0; n < DE3_ITER; n++)
    {
        boxFold(z);
        sphereFold(z, dr);

        z  = DE3_SCALE * z + offset;
        dr = dr * abs(DE3_SCALE) + 1.0f;

        min_dist = max(min_dist, length(z));
    }

    orbit = min_dist;

    float r = length(z);
    return r / abs(dr);
}

// ----------------------------------------------------------------------------
// domain repetition - 2008, 2013, 2023
// Inigo Quilez
// https://iquilezles.org/articles/sdfrepetition/
// p, input position
// s, scale/size
// returns, distance to surface
float repeated( vec3 p, float s )
{
    float id = round(p.y/s);
    float  o = sign(p.y-s*id); // neighbor offset direction
    
    float d = 1e20;
    for( int i=0; i<2; i++ )
    {
        float rid = id + float(i)*o;
        vec3 r = vec3(p.x, p.y - s*rid, p.z);
        d = min( d, DE(r) );
    }
    return d;
}

// ----------------------------------------------------------------------------
// p, input position
// o, output surface params
// returns, distance to surface
float getMap(in vec3 p, out surface o)
{
    float orbit;
    float distance = DE3(p, orbit);
    o = surface(
        0,
        1,
        orbit);
    return distance;
}

// ----------------------------------------------------------------------------
// p, input position
// returns, normal of closest surface
vec3 getNormal(in vec3 p)
{
    float h = MIN_DELTA;
    surface o;
    return normalize(vec3(
        getMap(p + vec3(h, 0.0, 0.0), o) - getMap(p - vec3(h, 0.0, 0.0), o),
        getMap(p + vec3(0.0, h, 0.0), o) - getMap(p - vec3(0.0, h, 0.0), o),
        getMap(p + vec3(0.0, 0.0, h), o) - getMap(p - vec3(0.0, 0.0, h), o)));
}

// ----------------------------------------------------------------------------
// p, input position
// d, input ray direction
// o, output surface params
// returns, distance to surface
float castRay(in vec3 p, in vec3 d, out surface o)
{
    float distance = 0.0;
    vec3 position = p;

    for (int i = 0; i < MAX_ITERATIONS; i++)
    {
        float delta = getMap(p + distance * d, o);
        distance += delta;

        if (distance > MAX_DISTANCE)
        {
            o.object = 0;
            o.iteration = i;
            return MAX_DISTANCE;
        }

        float m = mix(MIN_DELTA, MAX_DELTA, distance / 4.0);

        if (delta < m)
        {
            o.iteration = i;
            return distance;
        }
    }

    o.object = 0;
    o.iteration = MAX_ITERATIONS;
    return MAX_DISTANCE;
}

// ----------------------------------------------------------------------------
// Palettes
// Inigo Quilez
// https://www.shadertoy.com/view/ll2GD3
vec3 pal( in float t, in vec3 a, in vec3 b, in vec3 c, in vec3 d )
{
    return a + b*cos( 6.28318*(c*t+d) );
}

void main()
{
    const int aa = 1;
    const float b = float(aa);
    const float c = b * b;

    vec3 aa_color = vec3(0.0);

    for (int i = 0; i < aa; i++)
        for (int j = 0; j < aa; j++)
            {
                vec2 p = vec2(Resolution.z, 1.0) * FragCoord + vec2(i, j) / Resolution.xy / b;

                //vec3 origin = vec3(0.0, -mod(Time * 0.1, 2.0) + 1.0, 0.0);

                vec3 origin = (16.0 * ViewMatrix * vec4(vec3(0.0), 1.0)).xyz;
                vec3 direction = (ViewMatrix * vec4(normalize(vec3(p, -1.0)), 0.0)).xyz;

                //direction = vRotateZ(direction, Time * 0.1);
                //direction = vRotateX(direction, -M_PI / 2.0);

                surface object;
                float distance = castRay(origin, direction, object);

                gl_FragDepth = distance;
                
                float cheap_ao = 1.0 - float(object.iteration) / float(MAX_ITERATIONS);

                // https://www.shadertoy.com/view/ll2GD3
                // Line 28
                vec3 color = pal(
                    pow(cheap_ao, 0.8),
                    vec3(0.5 ,0.5 ,0.5),
                    vec3(0.5, 0.5, 0.5),
                    vec3(1.0 ,1.0 ,1.0),
                    vec3(0.3, 0.20, 0.20));
                color = mix(vec3(0.0), color, cheap_ao);

                aa_color += clamp(
                    pow(clamp(color, vec3(0.0), vec3(1.0)), vec3(1.0 / 2.2)),
                    vec3(0.0),
                    vec3(1.0)) / c;
            }

    FragColor = vec4(aa_color, 1.0);
}

