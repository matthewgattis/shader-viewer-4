#version 140

#define M_PI                (3.1415926535897932384626433832795)

uniform vec3 Resolution;
uniform float Time;
uniform mat4 ViewMatrix;

in vec2 FragCoord;
out vec4 FragColor;

#define MIN_DELTA       (1.0 / Resolution.y)
#define MAX_DELTA       (10.0 / Resolution.y)
#define MAX_DISTANCE    (32.0)

#define MAX_ITERATIONS  (32)

struct surface
{
    int object;
    int iteration;
    float orbit;
};

// ----------------------------------------------------------------------------
// distance functions
// Inigo Quilez
// https://iquilezles.org/articles/distfunctions/
float sdBox( vec3 p, vec3 b )
{
    vec3 q = abs(p) - b;
    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float doubleBox(vec3 p)
{
    const vec3 ONE = vec3(1.0);
    const vec3 HALF = vec3(0.5);
    
    vec3 offset = HALF;// * sin(Time / 10.0);

    return min(
        sdBox(p - offset, ONE),
        sdBox(p + offset, ONE));;
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
    vec3 id = round(p/s);
    vec3  o = sign(p-s*id); // neighbor offset direction
    
    float d = 1e20;
    for( int k=0; k<2; k++ )
    for( int j=0; j<2; j++ )
    for( int i=0; i<2; i++ )
    {
        vec3 rid = id + vec3(i,j,k)*o;
        vec3 r = p - s*rid;
        d = min( d, doubleBox(r) );
    }
    return d;
}

// ----------------------------------------------------------------------------
// p, input position
// o, output surface params
// returns, distance to surface
float getMap(in vec3 p, out surface o)
{
    o = surface(
        0,
        1,
        0.0f);

    return repeated(p, 8.0);
    //return doubleBox(p);
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
                vec2 p = FragCoord + vec2(i, j) / aa / Resolution.y;

                //vec3 origin = vec3(0.0, -mod(Time * 0.1, 2.0) + 1.0, 0.0);

                const float ZOOM = 4.0;
                vec3 origin = (ZOOM * ViewMatrix * vec4(vec3(0.0), 1.0)).xyz;
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
                    vec3(0.5,0.5,0.5),vec3(0.5,0.5,0.5),vec3(1.0,0.7,0.4),vec3(0.0,0.15,0.20));

                color = mix(vec3(0.0), color, cheap_ao);

                aa_color += clamp(
                    pow(clamp(color, vec3(0.0), vec3(1.0)), vec3(1.0 / 2.2)),
                    vec3(0.0),
                    vec3(1.0)) / c;
            }

    FragColor = vec4(aa_color, 1.0);
}

