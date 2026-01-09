#pragma once
#ifndef CONFIG_H
#define CONFIG_H

const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 1500;

struct MATRIX;

/// <summary>
/// 渲染模式
/// </summary>
enum class RendererType
{
	Single,
	Multiple
};

/// <summary>
/// 纹理模式
/// </summary>
enum class TextureType
{
	TEXTURE_2D,
	TEXTURE_CUBE_MAP,
	TEXTURE_FRAMEBUFFER
};

#endif