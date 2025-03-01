#include "gfx/gl/sampler.h"

#include <assert.h>

void GFX_GL_Sampler_Init(GFX_GL_Sampler *sampler)
{
    glGenSamplers(1, &sampler->id);
}

void GFX_GL_Sampler_Close(GFX_GL_Sampler *sampler)
{
    glDeleteSamplers(1, &sampler->id);
}

void GFX_GL_Sampler_Bind(GFX_GL_Sampler *sampler, GLuint unit)
{
    glBindSampler(unit, sampler->id);
}

void GFX_GL_Sampler_Parameteri(
    GFX_GL_Sampler *sampler, GLenum pname, GLint param)
{
    glSamplerParameteri(sampler->id, pname, param);
}

void GFX_GL_Sampler_Parameterf(
    GFX_GL_Sampler *sampler, GLenum pname, GLfloat param)
{
    glSamplerParameterf(sampler->id, pname, param);
}
