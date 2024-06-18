#include "MyShader_helper.h"
#include "BaseVSShader.h"
#include "commandbuilder.h"
#include "cpp_shader_constant_register_map.h"
#include "convar.h"

#include "SDK_screenspaceeffect_vs20.inc"
#include "my_pixelshader_ps20.inc"
#include "my_pixelshader_ps20b.inc"
#include "tier0/memdbgon.h"


void DrawInternalVolumetricShader(CBaseVSShader *pShader, IMaterialVar** params, IShaderDynamicAPI *pShaderAPI, IShaderShadow* pShaderShadow)
{
	// ----------------------------------------------------------------------------
	// This section is called when the shader is bound for the first time.
	// You should setup any static state variables here.
	// ----------------------------------------------------------------------------
	SHADOW_STATE
	{
		pShaderShadow->EnableTexture(SHADER_SAMPLER0, true);
		pShaderShadow->EnableSRGBRead(SHADER_SAMPLER0, true);

		// Setup the vertex format.
		int fmt = VERTEX_POSITION;
		pShaderShadow->VertexShaderVertexFormat(fmt, 1, 0, 0);

		// We don't need to write to the depth buffer.
		pShaderShadow->EnableDepthWrites(false);

		// Precache and set the screenspace vertex shader.
		DECLARE_STATIC_VERTEX_SHADER(sdk_screenspaceeffect_vs20);
		SET_STATIC_VERTEX_SHADER(sdk_screenspaceeffect_vs20);

		// Precache and set the pixel shader.
		if (g_pHardwareConfig->SupportsPixelShaders_2_b())
		{
			DECLARE_STATIC_PIXEL_SHADER(my_pixelshader_ps20b);
			SET_STATIC_PIXEL_SHADER(my_pixelshader_ps20b);
		}
		else
		{
			DECLARE_STATIC_PIXEL_SHADER(my_pixelshader_ps20);
			SET_STATIC_PIXEL_SHADER(my_pixelshader_ps20);
		}
	}

		// ----------------------------------------------------------------------------
		// This section is called every frame.
		// ----------------------------------------------------------------------------
		DYNAMIC_STATE
	{
		pShaderAPI->BindStandardTexture(SHADER_SAMPLER0, TEXTURE_FRAME_BUFFER_FULL_TEXTURE_0);

		// Use the sdk_screenspaceeffect_vs20 vertex shader.
		DECLARE_DYNAMIC_VERTEX_SHADER(sdk_screenspaceeffect_vs20);
		SET_DYNAMIC_VERTEX_SHADER(sdk_screenspaceeffect_vs20);

		// Use our custom pixel shader.
		if (g_pHardwareConfig->SupportsPixelShaders_2_b())
		{
			DECLARE_DYNAMIC_PIXEL_SHADER(my_pixelshader_ps20b);
			SET_DYNAMIC_PIXEL_SHADER(my_pixelshader_ps20b);
		}
		else
		{
			DECLARE_DYNAMIC_PIXEL_SHADER(my_pixelshader_ps20);
			SET_DYNAMIC_PIXEL_SHADER(my_pixelshader_ps20);
		}

		float vEyePos[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		pShaderAPI->GetWorldSpaceCameraPosition(vEyePos);
		pShaderAPI->SetPixelShaderConstant(5, vEyePos, 1);
		Msg("Camera Position: %f, %f, %f\n", vEyePos[0], vEyePos[1], vEyePos[2]);

		// Obtain the view matrix
		VMatrix viewMatrix;
		pShaderAPI->GetMatrix(MATERIAL_VIEW, viewMatrix.m[0]);

		// Compute the inverse of the view matrix
		VMatrix invViewMatrix;
		invViewMatrix.Identity();
		invViewMatrix = viewMatrix.InverseTR();

		Msg("View Matrix:\n");
		for (int i = 0; i < 4; ++i)
		{
			Msg("%f %f %f %f\n", viewMatrix[i][0], viewMatrix[i][1], viewMatrix[i][2], viewMatrix[i][3]);
		}

		// Set the inverse view matrix as a pixel shader constant
		pShaderAPI->SetPixelShaderConstant(0, invViewMatrix.Base(), 4); 
	}



		// NEVER FORGET THIS CALL! This is what actually
		// draws your shader!
	pShader->Draw();
}

void DrawVolumetricShader(CBaseVSShader *pShader, IMaterialVar** params, IShaderDynamicAPI *pShaderAPI, IShaderShadow* pShaderShadow)
{
	DrawInternalVolumetricShader(pShader, params, pShaderAPI, pShaderShadow);
}