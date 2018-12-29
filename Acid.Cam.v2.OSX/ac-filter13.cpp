/*
 * Software written by Jared Bruni https://github.com/lostjared
 
 This software is dedicated to all the people that struggle with mental illness.
 
 Website: http://lostsidedead.com
 YouTube: http://youtube.com/LostSideDead
 Instagram: http://instagram.com/jaredbruni
 Twitter: http://twitter.com/jaredbruni
 Facebook: http://facebook.com/LostSideDead0x
 
 You can use this program free of charge and redistrubute it online as long
 as you do not charge anything for this program. This program is meant to be
 100% free.
 
 BSD 2-Clause License
 
 Copyright (c) 2018, Jared Bruni
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 */

#include "ac.h"


void ac::ImageAlphaXorSubFilter(cv::Mat &frame) {
    if(blend_set == true && subfilter != -1 && ac::draw_strings[subfilter] != "ImageAlphaXorSubFilter") {
        cv::Mat copyf = frame.clone(), copyi = frame.clone();;
        cv::Mat resized;
        cv::resize(blend_image, resized, frame.size());
        cv::Mat out;
        CallFilter(subfilter, copyi);
        AlphaBlend(resized, copyi, out, 0.5);
        for(int z = 0; z < frame.rows; ++z) {
            for(int i = 0; i < frame.cols; ++i) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                cv::Vec3b pix = out.at<cv::Vec3b>(z, i);
                for(int j = 0; j < 3; ++j) {
                    pixel[j] = pixel[j]^pix[j];
                }
            }
        }
    }
    AddInvert(frame);
}

void ac::SmoothTrailsBlend(cv::Mat &frame) {
    static MatrixCollection<16> collection;
    cv::Mat copyf = frame.clone();
    cv::Mat copyi = frame.clone();
    Smooth(copyf, &collection);
    AlphaBlend(copyf, copyi, frame, 0.5);
    AddInvert(frame);
}

void ac::MatrixCollectionRGBXor(cv::Mat &frame) {
    static MatrixCollection<12> collection;
    cv::Mat copyf = frame.clone();
    MovementRGBTrails(copyf);
    collection.shiftFrames(copyf);
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Scalar sc;
            for(int q = 0; q < collection.size(); ++q) {
                cv::Vec3b pix = collection.frames[q].at<cv::Vec3b>(z, i);
                for(int j = 0; j < 3; ++j)
                    sc[j] += pix[j];
            }
            for(int j = 0; j < 3; ++j) {
                int val = static_cast<int>(sc[j]);
                pixel[j] = pixel[j]^val;
            }
        }
    }
    BlendWithSource(frame);
    AddInvert(frame);
}

void ac::RainbowGlitch(cv::Mat &frame) {
    SmoothTrailsBlend(frame);
    MovementRGBTrails(frame);
    AddInvert(frame);
}

void ac::RainbowGlichStrobe(cv::Mat &frame) {
    SmoothTrailsBlend(frame);
    MovementRGBTrails(frame);
    static bool negate = true;
    if(negate == true) {
        Negate(frame);
        negate = false;
    } else {
        negate = true;
    }
    AddInvert(frame);
}

void ac::NegateSwitchStrobe(cv::Mat &frame) {
    static bool strobe_value = true;
    if(strobe_value == true) {
        Negate(frame);
        strobe_value = false;
    } else
        strobe_value = true;
    
    AddInvert(frame);
}

void ac::StrobeAlphaShuffle(cv::Mat &frame) {
    cv::Mat copyf = frame.clone(), copyi = frame.clone();
    ShuffleAlpha(copyf);
    static bool neg_ = true;
    if(neg_ == true) {
        Negate(copyf);
        neg_ = false;
    } else {
        neg_ = true;
    }
    AlphaBlend(copyf, copyi, frame, 0.5);
    AddInvert(frame);
}

void ac::ShuffleAlphaWithRGB(cv::Mat &frame) {
    cv::Mat copy1 = frame.clone(), copy2 = frame.clone();
    ShuffleAlpha(copy1);
    ShuffleRGB(copy2);
    AlphaBlend(copy1, copy2, frame, 0.5);
    AddInvert(frame);
}

void ac::ShuffleAlphaSubFilter(cv::Mat &frame) {
    if(subfilter == -1 || ac::draw_strings[subfilter] == "ShuffleAlphaSubFilter")
        return;
    cv::Mat copyf = frame.clone(), copyi = frame.clone();
    ShuffleAlpha(copyf);
    CallFilter(subfilter, copyi);
    AlphaBlend(copyf, copyi, frame, 0.5);
    AddInvert(frame);
}

void ac::ShuffleColorMap(cv::Mat &frame) {
    static int index = 0;
    static std::vector<int> v{0,1,2,3,4,5,6,7,8,9,10,11};
    static auto rng = std::default_random_engine{};
    ac::setColorMap(v[index], frame);
    ++index;
    if(index > v.size()-1) {
        index = 0;
        std::shuffle(v.begin(), v.end(),rng);
    }
    AddInvert(frame);
}

void ac::BlendWithRainbowSubFilter(cv::Mat &frame) {
    if(subfilter == -1 || ac::draw_strings[subfilter] == "BlendWithRainbowSubFilter")
        return;
    blendFilterWithColorMap(subfilter, 4, frame);
    AddInvert(frame);
}

void ac::BlendWithJetSubFilter(cv::Mat &frame) {
    if(subfilter == -1 || ac::draw_strings[subfilter] == "BlendWithJetSubFilter")
        return;
    blendFilterWithColorMap(subfilter, 2, frame);
    AddInvert(frame);
}

void ac::ColormapBlendSubFilter(cv::Mat &frame) {
    if(subfilter == -1 || ac::draw_strings[subfilter] == "ColormapBlendSubFilter")
        return;
    cv::Mat copyf = frame.clone(), copyi = frame.clone();
    setColorMap(rand()%11, copyf);
    setColorMap(rand()%11, copyi);
    RGBColorTrails(copyi);
    CallFilter(subfilter, copyf);
    AlphaBlend(copyf, copyi, frame, 0.5);
    AddInvert(frame);
}

void ac::RandomColorMap(cv::Mat &frame) {
    setColorMap(rand()%11, frame);
    AddInvert(frame);
}

void ac::SmoothMirrorBlurFlip(cv::Mat &frame) {
    cv::Mat copy1 = frame.clone(), copy2 = frame.clone();
    SmoothTrailsBlend(copy1);
    MirrorBitwiseXor(copy1);
    SmoothTrailsBlend(copy2);
    BlurFlip(copy2);
    AlphaBlend(copy1, copy2, frame, 0.5);
    MedianBlend(frame);
    AddInvert(frame);
}

void ac::RandomColorMapAlphaBlendSubFilter(cv::Mat &frame) {
    if(subfilter == -1 || ac::draw_strings[subfilter] == "RandomColorMapAlphaBlendSubFilter")
        return;
    cv::Mat copy1 = frame.clone(), copy2 = frame.clone();
    RandomColorMap(copy1);
    CallFilter(subfilter, copy1);
    static double alpha = 1.0, alpha_max = 4.0;
    AlphaBlend(copy1, copy2, frame, alpha);
    static int dir = 1;
    procPos(dir, alpha, alpha_max, 4.1, 0.005);
    AddInvert(frame);
}

void ac::RandomOrder(cv::Mat &frame) {
    int color_order = 0;
    static std::vector<int> colors { 1,2,3,4 };
    static auto rng = std::default_random_engine{};
    static int index = static_cast<int>(colors.size()+1);
    if(index > colors.size()) {
        std::shuffle(colors.begin(), colors.end(),rng);
        index = 0;
    }
    color_order = colors[index++];
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &cur = frame.at<cv::Vec3b>(z, i);
            SwitchOrder(cur, color_order);
        }
    }
    AddInvert(frame);
}

void ac::RandomOrderMedianBlendSubFilter(cv::Mat &frame) {
    if(subfilter == -1 || ac::draw_strings[subfilter] == "RandomOrderMedianBlendSubFilter")
        return;
    cv::Mat copy1 = frame.clone(), copy2 = frame.clone();
    CallFilter(subfilter, copy1);
    AlphaBlend(copy1, copy2, frame, 0.5);
    MedianBlend(frame);
    RandomOrder(frame);
    AddInvert(frame);
}


void ac::MirrorOrder(cv::Mat &frame) {
    cv::Mat copy1 = frame.clone();
    static int index = 0;
    for(int z = 0; z < frame.rows-1; ++z) {
        for(int i = 0; i < frame.cols-1; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix[6];
            pix[0] = copy1.at<cv::Vec3b>(z, i);
            pix[1] = copy1.at<cv::Vec3b>(copy1.rows-z-1, i);
            pix[2] = copy1.at<cv::Vec3b>(copy1.rows-z-1, copy1.cols-i-1);
            pix[3] = copy1.at<cv::Vec3b>(z, copy1.cols-i-1);
            pix[4] = copy1.at<cv::Vec3b>(z+1, i+1);
            for(int j = 0; j < 3; ++j)
                SwitchOrder(pix[j], index);
            
            for(int j = 0; j < 3; ++j) {
                pixel[j] = pix[0][j] ^ pix[1][j] ^ pix[2][j] ^ pix[3][j] ^ pix[4][j] ^ pixel[j];
            }
        }
    }
    ++index;
    if(index > 4)
        index = 1;
    
    AddInvert(frame);
}

void ac::MirrorOrderSubFilter(cv::Mat &frame) {
    if(subfilter == -1 || ac::draw_strings[subfilter] == "MirrorOrderSubFilter")
        return;
    cv::Mat copy1 = frame.clone(), copy2 = frame.clone();
    CallFilter(subfilter, copy1);
    MirrorOrder(copy2);
    AlphaBlend(copy1, copy2, frame, 0.5);
    SmoothTrailsBlend(frame);
    DarkenImage(frame, 4);
    MedianBlend(frame);
    AddInvert(frame);
}

void ac::BlurMirrorOrder(cv::Mat &frame) {
    cv::Mat copy1 = frame.clone();
    MedianBlur(copy1);
    MedianBlur(copy1);
    MedianBlur(copy1);
    static double alpha = 1.0, alpha_max = 4.0;
    static int index = 0;
    for(int z = 0; z < frame.rows-1; ++z) {
        for(int i = 0; i < frame.cols-1; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix[6];
            pix[0] = copy1.at<cv::Vec3b>(z, i);
            pix[1] = copy1.at<cv::Vec3b>(copy1.rows-z-1, i);
            pix[2] = copy1.at<cv::Vec3b>(copy1.rows-z-1, copy1.cols-i-1);
            pix[3] = copy1.at<cv::Vec3b>(z, copy1.cols-i-1);
            pix[4] = copy1.at<cv::Vec3b>(z+1, i+1);
            for(int j = 0; j < 3; ++j)
                SwitchOrder(pix[j], index);
            for(int j = 0; j < 3; ++j) {
                pixel[j] = static_cast<unsigned char>(((pix[0][j] ^ pix[1][j] ^ pix[2][j] ^ pix[3][j] ^ pix[4][j] ^ pixel[j])/8) * alpha);
            }
        }
    }
    ++index;
    if(index > 4)
        index = 1;
    
    static int dir = 1;
    procPos(dir, alpha, alpha_max, 4.1, 0.05);
    MedianBlend(frame);
    AddInvert(frame);
}

void ac::AveragePixelMirror(cv::Mat &frame) {
    cv::Mat copy1 = frame.clone();
    MedianBlur(copy1);
    MedianBlur(copy1);
    static int index = 0;
    for(int z = 0; z < frame.rows-1; ++z) {
        for(int i = 0; i < frame.cols-1; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix[6];
            pix[0] = copy1.at<cv::Vec3b>(z, i);
            pix[1] = copy1.at<cv::Vec3b>(copy1.rows-z-1, i);
            pix[2] = copy1.at<cv::Vec3b>(copy1.rows-z-1, copy1.cols-i-1);
            pix[3] = copy1.at<cv::Vec3b>(z, copy1.cols-i-1);
            cv::Scalar s;
            for(int i = 0; i < 5; ++i) {
                for(int j = 0; j < 3; ++j) {
                    s[j] += pix[i][j];
                }
            }
            for(int j = 0; j < 3; ++j) {
                s[j] /= 4;
                unsigned int value1 = static_cast<unsigned int>(s[j]);
                unsigned int value2 = pixel[j]%value1;
                pixel[j] = pixel[j]^value2;
            }
        }
    }
    ++index;
    if(index > 4)
        index = 1;
    AddInvert(frame);
}

void ac::ShuffleAlphaMedianBlend(cv::Mat &frame) {
    cv::Mat copy1 = frame.clone(), copy2 = frame.clone();
    ShuffleAlpha(copy1);
    ShuffleAlpha(copy2);
    AlphaBlend(copy1, copy2, frame, 0.5);
    MedianBlend(frame);
    AddInvert(frame);
}

void ac::MirrorOrderAlpha(cv::Mat &frame) {
    cv::Mat copy1 = frame.clone(), copy2 = frame.clone();
    MirrorOrder(copy1);
    MirrorXorAll(copy2);
    AlphaBlend(copy1, copy2, frame, 0.5);
    MedianBlend(frame);
}

void ac::FilterStrobeSubFilter(cv::Mat &frame) {
    if(subfilter == -1 || ac::draw_strings[subfilter] == "FilterStrobeSubFilter")
        return;
    static bool flash = true;
    if(flash == true) {
        CallFilter(subfilter, frame);
        Negate(frame);
        flash = false;
    } else {
        flash = true;
        CallFilter(subfilter, frame);
    }
    AddInvert(frame);
}

void ac::ImageSubtractMedianBlend(cv::Mat &frame) {
    if(blend_set == true) {
        cv::Mat image1;
        cv::resize(blend_image, image1, frame.size());
        for(int z  = 0; z < frame.rows; ++z) {
            for(int i = 0; i < frame.cols; ++i) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                cv::Vec3b pix = image1.at<cv::Vec3b>(z, i);
                for(int j = 0; j < 3; ++j) {
                    pixel[j] -= (pixel[j] ^ pix[j])/6;
                }
            }
        }
        MedianBlend(frame);
    }
    AddInvert(frame);
}

void ac::ImageDarkBlend(cv::Mat &frame) {
    if(blend_set == true) {
        cv::Mat reimage;
        cv::resize(blend_image, reimage, frame.size());
        for(int z = 0; z < frame.rows; ++z) {
            cv::Scalar value;
            for(int i = 0; i < frame.cols; ++i) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                cv::Vec3b pix = reimage.at<cv::Vec3b>(z, i);
                for(int j = 0; j < 3; ++j) {
                    if(pix[j] == 0) pix[j] = 1;
                    unsigned int val = pixel[j]%(1+pix[j]);
                    pixel[j] = pixel[j]^val;
                }
            }
        }
        AddInvert(frame);
    }
}

void ac::ImageAverageDark(cv::Mat &frame) {
    if(blend_set == true) {
        cv::Mat reimage,frame_copy = frame.clone();
        cv::resize(blend_image,reimage, frame.size());
        for(int z = 0; z < frame.rows; ++z) {
            for(int i = 0; i < frame.cols; ++i) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                cv::Vec3b pix[5];
                pix[0] = frame_copy.at<cv::Vec3b>(z, i);
                pix[1] = frame_copy.at<cv::Vec3b>(z, frame.cols-i-1);
                pix[2] = frame_copy.at<cv::Vec3b>(frame.rows-z-1, i);
                pix[3] = frame_copy.at<cv::Vec3b>(frame.rows-z-1, frame.cols-i-1);
                cv::Scalar values;
                for(int j = 0; j < 4; ++j) {
                    for(int q = 0; q < 3; ++q) {
                        values[j] += pix[j][q];
                    }
                }
                cv::Vec3b color = reimage.at<cv::Vec3b>(z, i);
                for(int j = 0; j < 3; ++j) {
                    values[j] /= (frame.rows * frame.cols);
                    unsigned int value = static_cast<unsigned int>(values[j]);
                    color[j] /= 3;
                    pixel[j] += 50;
                    pixel[j] = color[j] ^ value ^ pixel[j];
                }
            }
        }
        AddInvert(frame);
    }
}

void ac::ImageRemainderPixel(cv::Mat &frame) {
    if(blend_set == true) {
        cv::Mat reimage;
        cv::resize(blend_image, reimage, frame.size());
        static double alpha = 1.0, alpha_max = 4.0;
        for(int z = 0; z < frame.rows; ++z) {
            for(int i = 0; i < frame.cols; ++i) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                cv::Vec3b pix = reimage.at<cv::Vec3b>(z, i);
                for(int j = 0; j < 3; ++j) {
                    unsigned int val = static_cast<unsigned int>(pixel[j] * alpha)%(1+pix[j]);
                    pixel[j] = pixel[j] ^ val;
                }
            }
        }
        static int dir = 1;
        procPos(dir,alpha,alpha_max,4.1, 0.05);
        AddInvert(frame);
    }
}

void ac::AverageLinesBlend(cv::Mat &frame) {
    static double alpha = 1.0, alpha_max = 4.0;
    for(int z = 0; z < frame.rows; ++z) {
        unsigned int values[3] = {0, 0, 0};
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b pixel = frame.at<cv::Vec3b>(z, i);
            for(int j = 0; j < 3; ++j) {
                values[j] += pixel[j];
            }
        }
        values[0] /= frame.cols;
        values[1] /= frame.cols;
        values[2] /= frame.cols;
        for(int i = 0; i < frame.cols; ++i ){
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            for(int j = 0; j < 3; ++j) {
                if(values[j] == 0 || values[j] == 255) values[j] = 1;
                pixel[j] += static_cast<unsigned char>((pixel[j]%1+values[j])*alpha);
            }
        }
    }
    static int dir = 1;
    procPos(dir, alpha, alpha_max, 4.1, 0.05);
    AddInvert(frame);
}

void ac::SoftFeedbackMirror(cv::Mat &frame) {
    SoftFeedbackResize64(frame);
    MirrorBitwiseXor(frame);
    AddInvert(frame);
}

void ac::AverageVerticalLinesBlend(cv::Mat &frame) {
    static double alpha = 1.0, alpha_max = 4.0;
    for(int i = 0; i < frame.cols; ++i) {
        unsigned int values[3] = {0, 0, 0};
        for(int z = 0; z < frame.rows; ++z) {
            cv::Vec3b pixel = frame.at<cv::Vec3b>(z, i);
            for(int j = 0; j < 3; ++j) {
                values[j] += pixel[j];
            }
        }
        values[0] /= frame.cols;
        values[1] /= frame.cols;
        values[2] /= frame.cols;
        for(int z = 0; z < frame.rows; ++z) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            for(int j = 0; j < 3; ++j) {
                if(values[j] == 0 || values[j] == 255) values[j] = 1;
                pixel[j] += static_cast<unsigned char>((pixel[j]%1+values[j])*alpha);
            }
        }
    }
    static int dir = 1;
    procPos(dir, alpha, alpha_max, 4.1, 0.05);
    AddInvert(frame);
}

void ac::LinesMedianBlend(cv::Mat &frame) {
    AverageLinesBlend(frame);
    AverageVerticalLinesBlend(frame);
    MedianBlend(frame);
    AddInvert(frame);
}

void ac::XorSquare(cv::Mat &frame) {
    for(int i = 0; i < frame.cols; ++i) {
        for(int z = 0; z < frame.rows; ++z) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            for(int j = 0; j < 3; ++j) {
                pixel[j] = pixel[j] ^ i ^ z;
            }
        }
    }
    AddInvert(frame);
}

void ac::PixelValuesPlusOne(cv::Mat &frame) {
    cv::Mat copy1 = frame.clone();
    MedianBlur(copy1);
    MedianBlur(copy1);
    MedianBlur(copy1);
    static double alpha = 1.0, alpha_max = 4.0;
    for(int i = 0; i < frame.cols-1; ++i) {
        for(int z = 0; z < frame.rows-1; ++z) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix[4];
            pix[0] = pixel;
            pix[1] = copy1.at<cv::Vec3b>(z+1, i+1);
            pix[2] = copy1.at<cv::Vec3b>(z+1, i);
            pix[3] = copy1.at<cv::Vec3b>(z, i+1);
            cv::Scalar values;
            for(int q = 0; q < 4; ++q) {
            	for(int j = 0; j < 3; ++j) {
                    values[j] += pix[q][j];
            	}
            }
            for(int j = 0; j < 3; ++j) {
                unsigned int v = static_cast<unsigned int>(values[j] * alpha);
                pixel[j] = pixel[j] ^ v;
            }
        }
    }
    static int dir = 1;
    procPos(dir, alpha, alpha_max, 4.1, 0.05);
    AddInvert(frame);
}

void ac::AverageHorizontalFilter(cv::Mat &frame) {
    static double alpha = 1.0, alpha_max = 4.0;
    for(int z = 0; z < frame.rows; ++z) {
        unsigned int values[3] = {0,0,0};
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b pixel = frame.at<cv::Vec3b>(z, i);
            for(int j = 0; j < 3; ++j) {
                values[j] += pixel[j];
            }
        }
        for(int j = 0; j < 3; ++j) {
            values[j] /= frame.cols;
        }
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            for(int j = 0; j < 3; ++j) {
                pixel[j] = static_cast<unsigned char>(pixel[j] * alpha) ^ static_cast<unsigned char>(values[j] * alpha);
            }
        }
    }
    static int dir = 1;
    procPos(dir, alpha, alpha_max, 4.1, 0.05);
    AddInvert(frame);
}

void ac::AverageVerticalFilter(cv::Mat &frame) {
    static double alpha = 1.0, alpha_max = 4.0;
    for(int i = 0; i < frame.cols; ++i) {
        unsigned int values[3] = {0,0,0};
        for(int z = 0; z < frame.rows; ++z) {
            cv::Vec3b pixel = frame.at<cv::Vec3b>(z, i);
            for(int j = 0; j < 3; ++j) {
                values[j] += pixel[j];
            }
        }
        for(int j = 0; j < 3; ++j) {
            values[j] /= frame.rows;
        }
        for(int z = 0; z < frame.rows; ++z) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            for(int j = 0; j < 3; ++j) {
                pixel[j] = static_cast<unsigned char>(pixel[j] * alpha) ^ static_cast<unsigned char>(values[j] * alpha);
            }
        }
    }
    static int dir = 1;
    procPos(dir, alpha, alpha_max, 4.1, 0.05);
    AddInvert(frame);
}

void ac::GradientAlphaXorHorizontal(cv::Mat &frame) {
    int iteration = frame.cols/255;
    static double alpha = 1.0, alpha_max = 4.0;
    for(int z = 0; z < frame.rows; ++z) {
        int index = 0, index_value = 0;
        for(int i = 0; i < frame.cols; ++i) {
            index ++;
            if((index%iteration)==0) {
                index = 0;
                index_value ++;
            }
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            for (int j = 0; j < 3; ++j) {
                pixel[j] = static_cast<unsigned char>(pixel[j]*alpha)^static_cast<unsigned char>(index_value*alpha);
            }
        }
    }
    static int dir = 1;
    procPos(dir, alpha, alpha_max, 4.1, 0.01);
    AddInvert(frame);
}

void ac::GradientAlphaXorVertical(cv::Mat &frame) {
    int iteration = frame.rows/255;
    static double alpha = 1.0, alpha_max = 4.0;
    for(int i = 0; i < frame.cols; ++i) {
        int index = 0, index_value = 0;
        for(int z = 0; z < frame.rows; ++z) {
            index ++;
            if((index%iteration)==0) {
                index = 0;
                index_value ++;
            }
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            for (int j = 0; j < 3; ++j) {
                pixel[j] = static_cast<unsigned char>(pixel[j]*alpha)^static_cast<unsigned char>(index_value*alpha);
            }
        }
    }
    static int dir = 1;
    procPos(dir, alpha, alpha_max, 4.1, 0.01);
    AddInvert(frame);
}

void ac::BlendImageWithSubFilter(cv::Mat &frame) {
    if(blend_set == false || subfilter == -1 || ac::draw_strings[subfilter] == "BlendImageWithSubFilter")
        return;
    cv::Mat copy1 = frame.clone();
    cv::Mat reimage;
    cv::resize(blend_image, reimage, frame.size());
    CallFilter(subfilter, copy1);
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix[3];
            pix[0] = pixel;
            pix[1] = copy1.at<cv::Vec3b>(z, i);
            pix[2] = reimage.at<cv::Vec3b>(z, i);
            unsigned int values[3] = {0,0,0};
            for(int q = 0; q < 3; ++q) {
                for(int j = 0; j < 3; ++j) {
                    values[j] += pix[q][j];
                }
            }
            for(int j = 0; j < 3; ++j) {
                values[j] /= 3;
                pixel[j] = pixel[j] ^ values[j];
            }
        }
    }
    AddInvert(frame);
}

void ac::BlendImageWithSubFilterAlpha(cv::Mat &frame) {
    if(blend_set == false || subfilter == -1 || ac::draw_strings[subfilter] == "BlendImageWithSubFilter")
        return;
    static double alpha = 1.0, alpha_max = 4.0;
    cv::Mat copy1 = frame.clone();
    cv::Mat reimage;
    cv::resize(blend_image, reimage, frame.size());
    CallFilter(subfilter, copy1);
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix[3];
            pix[0] = pixel;
            pix[1] = copy1.at<cv::Vec3b>(z, i);
            pix[2] = reimage.at<cv::Vec3b>(z, i);
            unsigned int values[3] = {0,0,0};
            for(int q = 0; q < 3; ++q) {
                for(int j = 0; j < 3; ++j) {
                    values[j] += pix[q][j];
                }
            }
            for(int j = 0; j < 3; ++j) {
                values[j] /= 3;
                pixel[j] = static_cast<unsigned char>(pixel[j]*alpha) ^ static_cast<unsigned char>(values[j]*alpha);
            }
        }
    }
    static int dir = 1;
    procPos(dir, alpha, alpha_max, 4.1, 0.01);
    AddInvert(frame);
}


void ac::MedianBlendSoft(cv::Mat &frame) {
    static MatrixCollection<8> collection;
    MedianBlur(frame);
    collection.shiftFrames(frame);
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Scalar value;
            for(int j = 0; j < collection.size(); ++j) {
                cv::Vec3b pixel = collection.frames[j].at<cv::Vec3b>(z, i);
                for(int q = 0; q < 3; ++q) {
                    value[q] += pixel[q];
                }
            }
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            for(int j = 0; j < 3; ++j) {
                int val = 1+static_cast<int>(value[j]);
                pixel[j] = static_cast<unsigned char>(pixel[j] ^ val);
            }
            swapColors(frame, z, i);// swap colors
            if(isNegative) invert(frame, z, i);// if isNegative invert pixel */
        }
    }
}

void ac::AndImageSubFilterXor(cv::Mat &frame) {
    if(blend_set == false || subfilter == -1 || ac::draw_strings[subfilter] == "AndImageSubFilterXor")
        return;
    cv::Mat copy1 = frame.clone(), reimage;
    cv::resize(blend_image, reimage, frame.size());
    CallFilter(subfilter, copy1);
    static double alpha = 1.0, alpha_max = 4.0;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix[2];
            pix[0] = reimage.at<cv::Vec3b>(z, i);
            pix[1] = copy1.at<cv::Vec3b>(z, i);
            unsigned int value = 0;
            for(int j = 0; j < 3; ++j) {
                value = pix[0][j] & pix[1][j];
                pixel[j] = pixel[j] ^ value;
            }
            
        }
    }
    static int dir = 1;
    procPos(dir, alpha, alpha_max, 4.1, 0.01);
    AddInvert(frame);
}

void ac::AlphaBlendImageSubFilterXor(cv::Mat &frame) {
    if(blend_set == false || subfilter == -1 || ac::draw_strings[subfilter] == "AlphaBlendImageSubFilterXor")
        return;
    cv::Mat reimage;
    cv::Mat copy1 = frame.clone(), copy2;
    cv::resize(blend_image, reimage, frame.size());
    CallFilter(subfilter, copy1);
    AlphaBlend(copy1, reimage, copy2, 0.5);
    Xor(frame, copy2);
    AddInvert(frame);
}

void ac::AlphaBlendImageSubFilterXorRev(cv::Mat &frame) {
    if(blend_set == false || subfilter == -1 || ac::draw_strings[subfilter] == "AlphaBlendImageSubFilterXorRev")
        return;
    cv::Mat reimage, copy1;
    cv::resize(blend_image, reimage, frame.size());
    CallFilter(subfilter, reimage);
    AlphaBlend(frame, reimage, copy1, 0.5);
    Xor(frame, copy1);
    AddInvert(frame);
}

void ac::ParticleReleaseXor(cv::Mat &frame) {
    cv::Mat copy1 = frame.clone();
    ParticleRelease(copy1);
    static double alpha = 1.0, alpha_max = 4.0;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix = copy1.at<cv::Vec3b>(z, i);
            for(int j = 0; j < 3; ++j) {
                pixel[j] = static_cast<unsigned char>(pixel[j] * alpha) ^ static_cast<unsigned char>(pix[j] * alpha);
            }
        }
    }
    static int dir = 1;
    procPos(dir, alpha, alpha_max, 4.1, 0.05);
}

void ac::ParticleReleaseXorVec(cv::Mat &frame) {
    cv::Mat copy1 = frame.clone();
    ParticleRelease(copy1);
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix = copy1.at<cv::Vec3b>(z, i);
            for(int j = 0; j < 3; ++j) {
                pixel[j] = pixel[j]^pix[j];
            }
        }
    }
    
}

void ac::ParticleReleaseAlphaBlend(cv::Mat &frame) {
    cv::Mat copy1 = frame.clone();
    ParticleRelease(copy1);
    static double alpha = 1.0, alpha_max = 4.0;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix = copy1.at<cv::Vec3b>(z, i);
            for(int j = 0; j < 3; ++j) {
                pixel[j] = static_cast<unsigned char>(pixel[j]*alpha) + static_cast<unsigned char>(pix[j]*alpha);
            }
        }
    }
    static int dir = 1;
    procPos(dir, alpha, alpha_max, 4.1, 0.01);
}

void ac::ParticleReleaseWithImage(cv::Mat &frame) {
    if(blend_set == false)
        return;
    cv::Mat copy1;
    cv::Mat copy2 = frame.clone();
    cv::resize(blend_image, copy1, frame.size());
    ParticleRelease(copy1);
    AlphaBlend(copy1, copy2, frame, 0.5);
    MedianBlend(frame);
}

void ac::ParticleReleaseSubFilter(cv::Mat &frame) {
    if(subfilter == -1 || ac::draw_strings[subfilter] == "ParticleReleaseSubFilter")
        return;
    cv::Mat copy1 = frame.clone(), copy2 = frame.clone();
    CallFilter(subfilter, copy2);
    AlphaBlend(copy1, copy2, frame, 0.5);
    MedianBlend(frame);
    ParticleRelease(frame);
}

void ac::ParticleReleaseImageSubFilter(cv::Mat &frame) {
    if(blend_set == false || subfilter == -1 || ac::draw_strings[subfilter] == "ParticleReleaseImageSubFilter")
        return;
    cv::Mat copy1 = frame.clone(), reimage;
    cv::resize(blend_image, reimage, frame.size());
    CallFilter(subfilter, reimage);
    AlphaBlend(copy1, reimage, frame, 0.5);
    ParticleRelease(frame);
    MedianBlend(frame);
}

void ac::ImageEnergy(cv::Mat &frame) {
    if(blend_set == false)
        return;
    pushSubFilter(filter_map["ExactImage"]);
    SmoothSubFilter32(frame);
    MedianBlend(frame);
    popSubFilter();
}


void ac::ImageEnergySubFilter(cv::Mat &frame) {
    if(blend_set == false || subfilter == -1 || ac::draw_strings[subfilter] == "ImageEnergySubFilter")
        return;
    SmoothSubFilter32(frame);
    SmoothImageAlphaBlend(frame);
    MedianBlend(frame);
}

void ac::ImageDistortion(cv::Mat &frame) {
    if(blend_set == false)
        return;
    cv::Mat copy1 = frame.clone(), copy2;
    cv::resize(blend_image, copy2, frame.size());
    pushSubFilter(ac::filter_map["SmoothImageAlphaBlend"]);
    EnergizeSubFilter(copy1);
    popSubFilter();
    pushSubFilter(ac::filter_map["ExactImage"]);
    SmoothSubFilter(copy2);
    popSubFilter();
    AlphaBlend(copy1, copy2, frame, 0.5);
}
