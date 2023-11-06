#include "vulkan_wrappers/vulkan_graphics_pipeline.hpp"

#include "utils.hpp"

#define OK_PRINT (A_YELLOW "[VULKAN GRAPHICS PIPELINE] " A_RESET)
#define BAD_PRINT (A_RED "[VULKAN GRAPHICS PIPELINE] " A_RESET)

using namespace rndrboi;

GraphicsPipeline::GraphicsPipeline()
{

}

void GraphicsPipeline::create( VulkanDevice& dev, RenderPass& render_pass, GraphicsPipelineSettings settings_in )
{

    if( settings_in.blend_type != OFF )
        std::cout << BAD_PRINT << " ONLY BLEND MODE OFF SUPPORTED NOW\n";

    settings = settings_in;
    dev_internal = &dev;

    vert_shader = rndrboi::utils::read_file( settings.vert_shader_path );
    frag_shader = rndrboi::utils::read_file( settings.frag_shader_path );

    vert_module = create_shader_module( dev, vert_shader );
    frag_module = create_shader_module( dev, frag_shader );

    VkPipelineShaderStageCreateInfo vert_shader_stage_info{};
    vert_shader_stage_info.sType    = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_shader_stage_info.stage    = VK_SHADER_STAGE_VERTEX_BIT;
    vert_shader_stage_info.module   = vert_module;
    vert_shader_stage_info.pName    = settings.vert_entrypoint.c_str();

    VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
    frag_shader_stage_info.sType    = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_shader_stage_info.stage    = VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_shader_stage_info.module   = frag_module;
    frag_shader_stage_info.pName    = settings.frag_entrypoint.c_str();

    VkPipelineShaderStageCreateInfo shader_stages[] = {
        vert_shader_stage_info,
        frag_shader_stage_info
    };

    // get the binding description for the vertex we are gonna use
    VkVertexInputBindingDescription binding_description = Vertex::binding_description();
    binding_description.binding = 0;

    // create the pos/col/norm/uv attributes from settings
    std::vector<VkVertexInputAttributeDescription> v_attribute_descr;

    for( auto& attr : settings.shader_attributes )
    {
        VkVertexInputAttributeDescription v_attr;
        v_attr.binding  = 0;
        v_attr.location = attr.location;
        v_attr.format   = attr.format;
        v_attr.offset   = attr.offset;
        v_attribute_descr.push_back(v_attr);
    }

    // vertex binding here / or instancing here
    VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info{};
    vertex_input_state_create_info.sType                = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_state_create_info.vertexBindingDescriptionCount    = v_attribute_descr.size() > 0 ? 1 : 0;
    vertex_input_state_create_info.pVertexBindingDescriptions       = &binding_description;
    vertex_input_state_create_info.vertexAttributeDescriptionCount  = v_attribute_descr.size();
    vertex_input_state_create_info.pVertexAttributeDescriptions     = v_attribute_descr.data();

    // vertex draw type
    VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info{};
    input_assembly_create_info.sType            = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_create_info.topology         = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly_create_info.primitiveRestartEnable   = VK_FALSE;

    viewport.x      = 0.0f;
    viewport.y      = 0.0f;
    viewport.width  = (float) settings.viewport_width;
    viewport.height = (float) settings.viewport_height;
    viewport.minDepth   = 0.0f;
    viewport.maxDepth   = 1.0f;

    scissor.offset = { 0, 0 };
    scissor.extent = { (uint32_t)settings.viewport_width, (uint32_t)settings.viewport_height };


    VkPipelineDynamicStateCreateInfo dynamic_state_create_info{};
    dynamic_state_create_info.sType     = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state_create_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
    dynamic_state_create_info.pDynamicStates    = dynamic_states.data();

    // dynamic state viewport
    VkPipelineViewportStateCreateInfo viewport_state_create_info{};
    viewport_state_create_info.sType        = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_state_create_info.viewportCount    = 1;
    viewport_state_create_info.scissorCount = 1;

    // rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer_create_info{};
    rasterizer_create_info.sType            = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer_create_info.depthClampEnable     = VK_FALSE;
    rasterizer_create_info.rasterizerDiscardEnable  = VK_FALSE;
    rasterizer_create_info.polygonMode          = settings.polygon_mode;
    rasterizer_create_info.lineWidth            = 1.0f;
    rasterizer_create_info.cullMode         = settings.cull_mode;
    rasterizer_create_info.frontFace            = settings.front_face;
    rasterizer_create_info.depthBiasEnable      = VK_FALSE;
    rasterizer_create_info.depthBiasConstantFactor  = 0.0f;
    rasterizer_create_info.depthBiasClamp       = 0.0f;
    rasterizer_create_info.depthBiasSlopeFactor     = 0.0f;

    // disable multisampling
    VkPipelineMultisampleStateCreateInfo multisampling_create_info{};
    multisampling_create_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling_create_info.sampleShadingEnable   = VK_FALSE;
    multisampling_create_info.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
    multisampling_create_info.minSampleShading      = 1.0f;     // Optional
    multisampling_create_info.pSampleMask       = nullptr;  // Optional
    multisampling_create_info.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling_create_info.alphaToOneEnable      = VK_FALSE; // Optional

    // depth and Stencil buffers
    // not used for now
    VkPipelineDepthStencilStateCreateInfo depth_stencil_create_info{};

    // alpha blend setup
    VkPipelineColorBlendAttachmentState color_blend_attachment_create_info{};
    color_blend_attachment_create_info.colorWriteMask       = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    if( settings.blend_type == ALPHA_BLEND )
    {
        color_blend_attachment_create_info.blendEnable      = VK_TRUE;
        color_blend_attachment_create_info.srcColorBlendFactor  = VK_BLEND_FACTOR_SRC_ALPHA;
        color_blend_attachment_create_info.dstColorBlendFactor  = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        color_blend_attachment_create_info.colorBlendOp     = VK_BLEND_OP_ADD;
        color_blend_attachment_create_info.srcAlphaBlendFactor  = VK_BLEND_FACTOR_ONE;
        color_blend_attachment_create_info.dstAlphaBlendFactor  = VK_BLEND_FACTOR_ZERO;
        color_blend_attachment_create_info.alphaBlendOp     = VK_BLEND_OP_ADD;
    }
    else
    {
        color_blend_attachment_create_info.blendEnable      = VK_FALSE;
    }

    VkPipelineColorBlendStateCreateInfo color_blend_create_info{};
    color_blend_create_info.sType       = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend_create_info.logicOpEnable   = settings.blend_type == OFF ? VK_FALSE : VK_TRUE;
    color_blend_create_info.logicOp     = VK_LOGIC_OP_COPY; // Optional
    color_blend_create_info.attachmentCount = 1;
    color_blend_create_info.pAttachments    = &color_blend_attachment_create_info;
    color_blend_create_info.blendConstants[0]   = 0.0f;         // Optional
    color_blend_create_info.blendConstants[1]   = 0.0f;         // Optional
    color_blend_create_info.blendConstants[2]   = 0.0f;         // Optional
    color_blend_create_info.blendConstants[3]   = 0.0f;         // Optional

    // layout
    VkPipelineLayoutCreateInfo pipeline_layout_create_info{};
    pipeline_layout_create_info.sType           = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_create_info.setLayoutCount      = settings.descriptor_layouts.size();
    pipeline_layout_create_info.pSetLayouts     = settings.descriptor_layouts.data();
    pipeline_layout_create_info.pushConstantRangeCount  = 0;        // Optional
    pipeline_layout_create_info.pPushConstantRanges = nullptr;  // Optional

    VkResult res = vkCreatePipelineLayout( dev.logical_device,
                                           &pipeline_layout_create_info,
                                           nullptr,
                                           &pipeline_layout );

    if( res != VK_SUCCESS )
        std::cout << BAD_PRINT << "ERROR Could not create pipeline layout\n";



    // create the pipeline
    VkGraphicsPipelineCreateInfo pipeline_create_info{};
    pipeline_create_info.sType          = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_create_info.stageCount     = 2;
    pipeline_create_info.pStages        = shader_stages;
    pipeline_create_info.pVertexInputState  = &vertex_input_state_create_info;
    pipeline_create_info.pInputAssemblyState    = &input_assembly_create_info;
    pipeline_create_info.pViewportState     = &viewport_state_create_info;
    pipeline_create_info.pRasterizationState    = &rasterizer_create_info;
    pipeline_create_info.pMultisampleState  = &multisampling_create_info;
    pipeline_create_info.pDepthStencilState = nullptr;
    pipeline_create_info.pColorBlendState   = &color_blend_create_info;
    pipeline_create_info.pDynamicState      = &dynamic_state_create_info;
    pipeline_create_info.layout         = pipeline_layout;
    pipeline_create_info.renderPass     = render_pass.render_pass;
    pipeline_create_info.subpass        = 0;
    pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
    pipeline_create_info.basePipelineIndex  = -1;

    res = vkCreateGraphicsPipelines( dev.logical_device,
                                     VK_NULL_HANDLE,
                                     1,
                                     &pipeline_create_info,
                                     nullptr,
                                     &pipeline);

    if( res != VK_SUCCESS )
        std::cout << BAD_PRINT << "ERROR Failed creating graphics pipeline\n";
    else
        std::cout << OK_PRINT << "Created Graphics Pipline\n";

}

void GraphicsPipeline::update_size( float width, float height )
{
    viewport.width = width;
    viewport.height = height;
    scissor.extent = { (uint32_t)viewport.width, (uint32_t)viewport.height };

    settings.viewport_width = width;
    settings.viewport_height = height;
}

void GraphicsPipeline::clean()
{

    vkDestroyPipeline( dev_internal->logical_device, pipeline, nullptr );
    vkDestroyPipelineLayout( dev_internal->logical_device, pipeline_layout, nullptr );
    vkDestroyShaderModule( dev_internal->logical_device, vert_module, nullptr );
    vkDestroyShaderModule( dev_internal->logical_device, frag_module, nullptr );
}

VkShaderModule GraphicsPipeline::create_shader_module( VulkanDevice& dev, std::vector<char>& code )
{
    VkShaderModule module;

    VkShaderModuleCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = code.size();
    create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkResult res = vkCreateShaderModule(dev.logical_device, &create_info, nullptr, &module);

    if( res != VK_SUCCESS )
        std::cout << BAD_PRINT << "Could not create shader module\n";

    return std::move(module);
}
