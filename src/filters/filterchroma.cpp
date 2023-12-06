#include "src/canvas2d.h"
#include "filterutils.h"
#include "src/settings.h"

std::vector<RGBA> chroma(std::vector<RGBA> &data, int width, int height) {

    std::vector<RGBA> result(data.size()); 
    std::vector<RGBA> resultX(data.size());

    int rS = settings.rShift;
    int bS = settings.bShift;
    int gS = settings.gShift;

    for (int y=0;y < height; y++) { // horizontal shift
        for (int x=0;x<width;x++) {

            resultX.at(width*y+x).r = FilterUtils::getPixelRepeated(data,width,height,x+rS,y).r;
            resultX.at(width*y+x).g = FilterUtils::getPixelRepeated(data,width,height,x+gS,y).g;
            resultX.at(width*y+x).b = FilterUtils::getPixelRepeated(data,width,height,x+bS,y).b;
            resultX.at(width*y+x).a = 255;
        }
    }

    for (int x=0;x < width; x++) { // vertical shift
        for (int y=0;y<height;y++) {

            result.at(width*y+x).r = FilterUtils::getPixelRepeated(resultX,width,height,x,y+rS).r;
            result.at(width*y+x).g = FilterUtils::getPixelRepeated(resultX,width,height,x,y+gS).g;
            result.at(width*y+x).b = FilterUtils::getPixelRepeated(resultX,width,height,x,y+bS).b;
            result.at(width*y+x).a = 255;

        }
    }
    return result;
}

void Canvas2D::filterChroma() {
    std::vector<RGBA> chromatic = chroma(m_data,m_width,m_height);
    for (int i=0; i<m_data.size();i++) {
        m_data[i] = chromatic[i];
    }
}
