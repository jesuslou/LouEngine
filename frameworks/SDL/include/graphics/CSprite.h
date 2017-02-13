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

#include <graphics/ISprite.h>

#include <SDL_render.h>

#include <memory>

struct SDL_Texture;

class CSprite : public ISprite
{
public:
	CSprite();
	~CSprite();

	bool LoadFromFile(const char* path) override;
	bool LoadFromMemory(void* buffer, std::size_t size) override;
	
	void Destroy() override;

	bool IsValid() const override;

	void SetPosition(const CVector2i& position) override;
	void SetAlpha(unsigned alpha) override;
	void SetAngle(double angle) override;
	void SetScale(const CVector2i& scale) override;
	void SetPivot(const CVector2i& pivot) override;
	//virtual void SetFlipMode(SDL_RendererFlip new_flip_mode) = 0;
	//virtual void SetTintColor(SDL_Color & new_color, bool overrides_alpha = false) = 0;
	//virtual void SetBlendMode(SDL_BlendMode new_blend_mode) = 0;
	//virtual void SetClip(SDL_Rect & new_clip) = 0;
	void SetWidth(int width) override;
	void SetHeight(int height) override;

	CVector2i& getPosition() const override;
	unsigned getAlpha() const override;
	double getAngle() const override;
	CVector2i getScale() const override;

	CVector2i getPivot() const override;
	//virtual SDL_RendererFlip getFlipMode() const = 0;
	//virtual SDL_Color getTintColor() const = 0;
	//virtual SDL_BlendMode getBlendMode() const = 0;
	//virtual SDL_Rect getClip() const = 0;

	unsigned getWidth() const override;
	unsigned getHeight() const override;

private:
	std::shared_ptr<SDL_Texture> m_texture;

	CVector2i m_position;
	unsigned m_alpha;
	double m_angle;
	CVector2i m_scale;

	CVector2i m_pivot;
	SDL_RendererFlip m_flip_mode;
	SDL_Color m_tint_color;
	SDL_BlendMode m_blend_mode;
	SDL_Rect m_clip;

	void* m_pixels;

	int m_pitch;
	unsigned m_width;
	unsigned m_height;

	std::string m_filepath;
};
