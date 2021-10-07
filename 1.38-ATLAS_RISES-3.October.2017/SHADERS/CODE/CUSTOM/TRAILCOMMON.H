//-----------------------------------------------------------------------------
///
///     CustomPerMaterialUniforms
///
///     @brief      CustomPerMaterialUniforms
///
///     Stuff that is only used for these types of meshes.
//-----------------------------------------------------------------------------
struct CustomPerMaterialUniforms
{
    SAMPLER2D( gDiffuseMap );
    vec4 gMinSizeInPixels;  
};

//
// This is the SRT buffer that everything gets uploaded to (on PS4). PC just functions as normal.
//
struct UniformBuffer
{
    //DECLARE_PTR( CommonPerMaterialUniforms, mpCommonPerMaterial )   /*: PER_MATERIAL*/ // sematics currently crash the compiler so the parser is hardcoded to look for names.  
    DECLARE_PTR( PerFrameUniforms,          mpPerFrame )   /*: PER_MATERIAL*/ // sematics currently crash the compiler so the parser is hardcoded to look for names.  
    DECLARE_PTR( CommonPerMeshUniforms,     mpCommonPerMesh )       /*: PER_MESH*/ // sematics currently crash the compiler so the parser is hardcoded to look for names.  
    DECLARE_PTR( CustomPerMaterialUniforms, mpCustomPerMaterial )       /*: PER_MESH*/
};