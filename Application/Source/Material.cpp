#include "Material.h"

Material::Material()
{
	kAmbient.Set(.1f, .1f, .1f);
	kDiffuse.Set(.6f, .6f, .6f);
	kSpecular.Set(.3f, .3f, .3f);
	kShininess = 2.f;
}