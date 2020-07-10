#include <VX/vx.h>
#include <VX/vxu.h>
#include <VX/vx_compatibility.h>
#include <VX/vx_lib_debug.h>
#include <VX/vx_lib_extras.h>
#include <VX/vx_lib_xyz.h>

#ifndef dimof
#define dimof(x)    (sizeof(x)/sizeof(x[0]))
#endif

int main(int argc, char* argv)
{
    vx_status status = VX_SUCCESS;
    vx_context context = vxCreateContext();

    vx_uint32 i = 0, w = 640, h = 480;
    vx_image images[] = {
        vxCreateImage(context, w, h, VX_DF_IMAGE_RGB),    /* 0: rgb */
        vxCreateImage(context, w, h, VX_DF_IMAGE_IYUV),   /* 1: yuv */
        vxCreateImage(context, w, h, VX_DF_IMAGE_U8),     /* 2: luma */
        vxCreateImage(context, w / 2, h / 2, VX_DF_IMAGE_U8), /* 3: u channel */
        vxCreateImage(context, w / 2, h / 2, VX_DF_IMAGE_U8), /* 4: v channel */
    };

    vx_graph graph = vxCreateGraph(context);
    if (vxGetStatus((vx_reference)graph) == VX_SUCCESS) {
        vx_node nodes[] = {
            vxFReadImageNode(graph, "colorbars_640x480_I444.rgb", images[0]),
            vxColorConvertNode(graph, images[0], images[1]),
            vxFWriteImageNode(graph, images[1], "oiyuv_640x480_P420.yuv"),
            vxChannelExtractNode(graph, images[1], VX_CHANNEL_Y, images[2]),
            vxChannelExtractNode(graph, images[1], VX_CHANNEL_U, images[3]),
            vxChannelExtractNode(graph, images[1], VX_CHANNEL_V, images[4]),
            vxFWriteImageNode(graph, images[2], "oy_640x480_P400.bw"),
            vxFWriteImageNode(graph, images[3], "ou_320x240_P400.bw"),
            vxFWriteImageNode(graph, images[4], "ov_320x240_P400.bw"),
        };
        if (status == VX_SUCCESS) {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS) {
                status = vxProcessGraph(graph);
            }
        }
        for (i = 0; i < dimof(nodes); i++) {
            vxReleaseNode(&nodes[i]);
        }
        vxReleaseGraph(&graph);
    }
    return 0;
}