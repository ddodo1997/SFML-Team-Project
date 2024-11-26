#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject(const std::string& name)
	: name(name), originPreset(Origins::TL)
{

}

void GameObject::SetOrigin(Origins preset)
{
	originPreset = preset;
	origin = { 0.f, 0.f };
}

void GameObject::Reset()
{
	isDevMode = false;
}

void GameObject::OnLocallize(Languages lang)
{
	Variables::currentLang = lang;
	STRING_TABLE->SetPath();
}
