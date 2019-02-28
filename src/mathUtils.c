
#include "MathUtils.h"
float minf(float a, float b)
{
    if (a < b)
        return (a);
    return (b);   
}

float maxf(float a, float b)
{
    if (a > b)
        return (a);
    return (b);   
}

float clampf(float a, float min, float max)
{
    return (minf(maxf(a, min), max));  
}

float crossf(float x0, float y0, float x1, float y1)
{
    return (x0 * y1 - x1 * y0);  
}

int overlapf(float begin0, float end0, float begin1, float end1)
{
    return (minf(begin0, end0) <= maxf(begin1, end1)
                && minf(begin1, end1) <= maxf(begin0, end0));  
}

int intersectBox(t_vertex v1, t_vertex v2, t_vertex v3, t_vertex v4)
{
    return (overlapf(v1.x, v2.x, v3.x, v4.x)
                                && overlapf(v1.y, v2.y, v3.y, v4.y));
}

t_vertex intersectLine(t_vertex v1, t_vertex v2, t_vertex v3, t_vertex v4)
{
    t_vertex result;

    result.x = crossf(crossf(v1.x, v1.y, v2.x, v2.y),
                    v1.x-v2.x, crossf(v3.x, v3.y, v4.x, v4.y),v3.x- v4.x) /
                    crossf(v1.x - v2.x, v1.y - v2.x, v3.x - v4.x, v3.y - v4.y);
    result.y = crossf(crossf(v1.x, v1.y, v2.x, v2.y), v1.y-v2.y,
                    crossf(v3.x, v3.y, v4.x, v4.y), v3.y- v4.y) /
                    crossf(v1.x - v2.x, v1.y - v2.x, v3.x - v4.x, v3.y - v4.y);
    return (result);
}

float pointSide(t_vertex p, t_vertex v1, t_vertex v2)
{
    return crossf(v2.x - v1.x, v2.y - v1.y, p.x - v1.x, p.y - v1.y);
}

float calcYaw(float y, float z, float currentYaw)
{
    return (y + z * currentYaw);
}
