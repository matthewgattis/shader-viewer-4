#version 140

//uniform vec2 Resolution;
//uniform float Time;

in vec2 ex_Position;

//void main()
//{
    //vec2 p = vec2(Resolution.x / Resolution.y, 1.0) * ex_Position;
    //gl_FragColor = vec4(p.x, 1.0, 1.0, 1.0);
//}

// MG - Playing with Kaleidoscopic IFS
// http://www.fractalforums.com/ifs-iterated-function-systems/kaleidoscopic-(escape-time-ifs)/
// http://blog.hvidtfeldts.net/index.php/2011/06/distance-estimated-3d-fractals-part-i/
// If you know how to fix that artifacting, don't keep it to yourself.
// It comes as a result of modulo.
#ifdef GL_ES
precision mediump float;
#endif

uniform float Time;
uniform vec2 Resolution;

uniform mat4 eye;
uniform vec3 eyeTranslate;
uniform vec3 eyeRotate;

#define M_PI                (3.1415926535897932384626433832795)

// Ray Marcher
#define MINDELTA            (0.00001)
#define MAXDELTA            (0.01)
#define MAXDIST             (8.0)
// Provide 'float s', a distance.
#define PRECISION           (mix(MINDELTA, MAXDELTA, s/MAXDIST))
#define MAXITER             (32)

// Background
#define LIGHTVEC            (vec3(1.0, 1.0, 0.1))
#define BACKROUND1          (vec3(0.6, 0.8, 1.0))
#define BACKROUND2          (vec3(0.6, 0.8, 1.0)/2.0)

#define USE_SIMPLEX_NOISE
#define SIMPLEX_ITERATIONS1     8
#define SIMPLEX_ITERATIONS2     1

#define FRACT_ITER          20
#define FRACT_SCALE         1.8
#define FRACT_OFFSET            1.0

vec3 vRotateX(vec3 p, float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return vec3(p.x, c*p.y+s*p.z, -s*p.y+c*p.z);
}

vec3 vRotateY(vec3 p, float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return vec3(c*p.x-s*p.z, p.y, s*p.x+c*p.z);
}

vec3 vRotateZ(vec3 p, float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return vec3(c*p.x+s*p.y, -s*p.x+c*p.y, p.z);
}

float DE(vec3 z) {
    float c = 2.0;
    //z.y = mod(z.y, c)-c/2.0;

    z = vRotateZ(z, M_PI/2.0);

    float r;
    int n1 = 0;
    for (int n = 0; n < FRACT_ITER; n++) {
        float rotate = M_PI*0.5;
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

// https://iquilezles.org/articles/sdfrepetition/
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

float getMap(in vec3 position, out int object, bool bump) {
    float distance = MAXDIST;
    float tempDist = MAXDIST;

    distance = repeated(position, 2.); object = 1;
    //distance = DE(position); object = 1;
    
    return distance;
}

vec3 getNormal(vec3 p) {
    vec3 s = p;
    float h = MINDELTA;
    int object;
    return normalize(vec3(
            getMap(p + vec3(h, 0.0, 0.0), object, true) - getMap(p - vec3(h, 0.0, 0.0), object, true),
            getMap(p + vec3(0.0, h, 0.0), object, true) - getMap(p - vec3(0.0, h, 0.0), object, true),
            getMap(p + vec3(0.0, 0.0, h), object, true) - getMap(p - vec3(0.0, 0.0, h), object, true)));
}

vec2 castRay(in vec3 origin, in vec3 direction, out int object) {
    float distance = 0.0;
    float delta = 0.0;
    vec3 position;
    object = 0;
    int iter = 0;
    
    position = origin;
    for (int i = 0; i < MAXITER; i++) {     
        iter += 1;
        delta = getMap(position, object, false);
        
        distance += delta;
        position = origin + direction*distance;
        float s = distance;
        if (delta <= PRECISION) {
            return vec2(distance, float(i));
        }
        if (distance > MAXDIST) {
            break;
        }
    }
    
    object = 0;
    return vec2(MAXDIST, float(iter));
}

vec3 getShading(in vec2 distance, in vec3 origin, in vec3 direction, in int object) {
    vec3 position = origin + direction * distance.x;
    vec3 color = vec3(1.0, 1.0, 1.0);
    
    float cheapAO = distance.y/float(MAXITER);
    
    if (object == 1) {
        color = vec3(1.0, 1.0, 1.0);
    }
    
    return mix(color, vec3(0.0, 0.0, 0.0), cheapAO);
}

vec3 drawScene(in vec3 origin, in vec3 direction) {
    int object = 0;
    
    vec2 distance = castRay(origin, direction, object);
    
    return getShading(distance, origin, direction, object);
}

void main() {

    vec2 p = vec2(Resolution.x / Resolution.y, 1.0) * ex_Position;

    //vec2 p=(gl_FragCoord.xy/resolution.y)*2.0;
    //p.x-=resolution.x/resolution.y*1.0;p.y-=1.0;

    //vec3 origin = vec3(0.0, 0.0, 0.0) - eyeTranslate/128.0;
    vec3 origin = vec3(0.0, -mod(Time*0.1, 2.0)+1.0, 0.0);
    vec3 direction = normalize(vec3(p.x,p.y,1.0));

    direction = vRotateZ(direction, Time*0.1);
    direction = vRotateX(direction, eyeRotate.x-M_PI/2.0);
    //direction = vRotateZ(direction, eyeRotate.z);

    vec3 color = drawScene(origin, direction);

    gl_FragColor = vec4(pow(color,vec3(1.0/2.2)), 1.0);
}

