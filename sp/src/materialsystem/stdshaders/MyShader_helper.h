#include <string.h>
#include "BaseVSShader.h"


class CBaseVSShader;
class IMaterialVar;
class IShaderDynamicAPI;
class IShaderShadow;


 void DrawVolumetricShader(CBaseVSShader *pShader, IMaterialVar** params, IShaderDynamicAPI *pShaderAPI, IShaderShadow* pShaderShadow);