////////////////////////////////////////////////////////////
//
// LouEngine - Multiplatform Game Engine Project
// Copyright (C) 2016-2017 Jesus Lou (loucocito@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#pragma once

class ISprite
{
public:
	ISprite() {}
	virtual ~ISprite() {}

	virtual bool LoadFromFile(const char* path) = 0;
	virtual bool LoadFromMemory(void* buffer, std::size_t size) = 0;
	
	virtual void Destroy() = 0;

	virtual bool IsValid() const = 0;

	virtual void SetPosition(const CVector2i& position) = 0;
	virtual void SetAlpha(unsigned alpha) = 0;
	virtual void SetAngle(double angle) = 0;
	virtual void SetScale(const CVector2i& scale) = 0;
	virtual void SetPivot(const CVector2i& pivot) = 0;
	//virtual void SetFlipMode(SDL_RendererFlip new_flip_mode) = 0;
	//virtual void SetTintColor(SDL_Color & new_color, bool overrides_alpha = false) = 0;
	//virtual void SetBlendMode(SDL_BlendMode new_blend_mode) = 0;
	//virtual void SetClip(SDL_Rect & new_clip) = 0;
	virtual void SetWidth(int width) = 0;
	virtual void SetHeight(int height) = 0;

	virtual CVector2i& getPosition() const = 0;
	virtual unsigned getAlpha() const = 0;
	virtual double getAngle() const = 0;
	virtual CVector2i getScale() const = 0;

	virtual CVector2i getPivot() const = 0;
	//virtual SDL_RendererFlip getFlipMode() const = 0;
	//virtual SDL_Color getTintColor() const = 0;
	//virtual SDL_BlendMode getBlendMode() const = 0;
	//virtual SDL_Rect getClip() const = 0;

	virtual unsigned getWidth() const = 0;
	virtual unsigned getHeight() const = 0;
};
