/* Acid Cam Functions for OpenCV
 * written by Jared Bruni https://github.com/lostjared
 
 This software is dedicated to all the people that struggle with mental illness.
 
 Website: http://lostsidedead.com
 YouTube: http://youtube.com/LostSideDead
 Instagram: http://instagram.com/jaredbruni
 Twitter: http://twitter.com/jaredbruni
 Facebook: http://facebook.com/LostSideDead0x
 
 You can use this program free of charge and redistrubute as long
 as you do not charge anything for this program. This program is 100%
 Free.
 
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

void ac::MoveUpLeft(cv::Mat &frame) {
    
    static double alpha = 1.0, alpha_max = 7.0;
    
    static cv::Mat old_frame = frame.clone();
    static cv::Size old_size = old_frame.size();
    
    if(reset_alpha == true || frames_released == true || old_size != frame.size()) {
        old_frame = frame.clone();
        old_size = old_frame.size();
    }
    for(int z = 0; z < old_frame.rows-1; ++z) {
        for(int i = 0; i < old_frame.cols-1; ++i) {
            cv::Vec3b &pixel = old_frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix = old_frame.at<cv::Vec3b>(z+1, i+1);
            pixel = pix;
        }
    }
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix = old_frame.at<cv::Vec3b>(z, i);
            for(int j = 0; j < 3; ++j)
                pixel[j] = static_cast<unsigned char>((pixel[j]^pix[j])*alpha);
        }
    }
    
    static int dir = 1;
    procPos(dir, alpha, alpha_max);
    AddInvert(frame);
}

void ac::RandomStrobe(cv::Mat &frame) {
    static double alpha = 1.0, alpha_max = 7.0;
    static std::vector<std::string> svStrobe{  "StrobeEffect", "Blank", "Type","Random Flash","Strobe Red Then Green Then Blue","Flash Black","FlashWhite","StrobeScan", "RGBFlash", "ReinterpretDouble", "DiamondStrobe", "BitwiseXorStrobe","FlashBlackAndWhite", "StrobeBlend", "FibFlash", "ScaleFlash", "FadeStrobe", "AndStrobe", "AndStrobeScale", "AndPixelStrobe", "AndOrXorStrobe", "AndOrXorStrobeScale", "BrightStrobe", "DarkStrobe", "RandomXorOpposite", "StrobeTransform"};
    
    cv::Mat old_frame = frame.clone();
    DrawFilter(svStrobe[rand()%svStrobe.size()], old_frame);
    cv::Mat copy = frame.clone();
    AlphaBlend(old_frame, copy, frame, alpha);
    static int dir = 1;
    procPos(dir, alpha, alpha_max, 15, 0.1);
    AddInvert(frame);
}

void ac::RandomBlur(cv::Mat &frame) {
    std::vector<std::string> svBlur { "GaussianBlur", "Median Blur", "Blur Distortion", "ColorTrails","TrailsFilter", "TrailsFilterIntense", "TrailsFilterSelfAlpha", "TrailsFilterXor","BlurSim", "TrailsInter", "TrailsBlend", "TrailsNegate", "AcidTrails", "HorizontalTrailsInter" ,"Trails", "BlendTrails", "SmoothTrails", "SmoothTrailsSelfAlphaBlend", "SmoothTrailsRainbowBlend", "MedianBlend", "XorTrails", "RainbowTrails", "NegativeTrails", "IntenseTrails", "GaussianBlend", "RandomAmountMedianBlur", "MedianBlendAnimation", "AlphaAcidTrails"};
    static double alpha = 1.0, alpha_max = 7.0;
    cv::Mat old_frame = frame.clone();
    DrawFilter(svBlur[rand()%svBlur.size()], old_frame);
    cv::Mat copy = frame.clone();
    AlphaBlend(old_frame, copy, frame, alpha);
    static int dir = 1;
    procPos(dir, alpha, alpha_max, 15, 0.1);
    AddInvert(frame);
}


void ac::StuckStrobe(cv::Mat &frame) {
    static double alpha = 1.0, alpha_max = 7.0;
    static cv::Mat old_frame = frame.clone();
    if(reset_alpha == true || frames_released == true || old_frame.size() != frame.size()) {
        old_frame = frame.clone();
    }
    cv::Mat copy = frame.clone();
    AlphaBlend(old_frame, copy, frame, alpha);
    static int dir = 1;
    procPos(dir, alpha, alpha_max, 15, 0.1);
    AddInvert(frame);
}

void ac::Stuck(cv::Mat &frame) {
    static cv::Mat old_frame = frame.clone();
    if(reset_alpha == true || frames_released == true || old_frame.size() != frame.size()) {
        old_frame = frame.clone();
    }
    Add(frame, old_frame);
    AddInvert(frame);
}

void ac::OrStrobe(cv::Mat &frame) {
    cv::Mat copy = frame.clone();
    static cv::Mat prev_frame = frame.clone();
    if(prev_frame.size() != frame.size()) {
        prev_frame = frame.clone();
    }
    cv::Vec3b randval(rand()%255, rand()%255, rand()%255);
    static double alpha = 1.0, alpha_max = 7.0;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix = prev_frame.at<cv::Vec3b>(z, i);
            for(int j = 0; j < 3; ++j) {
                pixel[j] = static_cast<unsigned char>((pixel[j]|randval[j]|pix[j])*alpha);
            }
        }
    }
    static int dir = 1;
    procPos(dir, alpha, alpha_max);
    prev_frame = copy;
    AddInvert(frame);
}

void ac::LagBlend(cv::Mat &frame) {
    static MatrixCollection<8> collection;
    static cv::Size c_size = frame.size();
    // lazy init
    static bool init = false;
    if(c_size != frame.size() || init == false) {
        collection.shiftFrames(frame);
        init = true;
        c_size = frame.size();
    }
    static int frame_count = 0;
    ++frame_count;
    if(frame_count > (ac::fps/8)) {
        collection.shiftFrames(frame);
        frame_count = 0;
    }
    for(int i = 0; i < collection.size(); ++i) {
        DarkenFilter(frame);
        Add(frame, collection.frames[i]);
    }
    AddInvert(frame);
}

void ac::SubFilter(cv::Mat &frame) {
    static MatrixCollection<8> collection;
    collection.shiftFrames(frame);
    for(int j = 0; j < collection.size(); ++j) {
        Sub(frame, collection.frames[j]);
    }
    AddInvert(frame);
}

void ac::AddFilter(cv::Mat &frame) {
    static MatrixCollection<12> collection;
    collection.shiftFrames(frame);
    for(int j = 0; j < collection.size(); ++j) {
        Add(frame, collection.frames[j]);
    }
    AddInvert(frame);
}

// Use in a custom with Darken Filter twice
void ac::RGBTrails(cv::Mat &frame) {
    static MatrixCollection<8> collection;
    collection.shiftFrames(frame);
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            int offset = 0;
            for(int j = 0; j < collection.size(); ++j) {
                cv::Vec3b pix = collection.frames[j].at<cv::Vec3b>(z, i);
                pixel[offset] = static_cast<unsigned char>(pixel[offset]+(pix[offset]));
                
                ++offset;
                if(offset > 2)
                    offset = 0;
            }
            swapColors(frame, z, i);
            if(isNegative) invert(frame, z, i);
        }
    }
}

void ac::RGBTrailsDark(cv::Mat &frame) {
    DarkenFilter(frame);
    DarkenFilter(frame);
    RGBTrails(frame);
}

void ac::RGBTrailsAlpha(cv::Mat &frame) {
    static MatrixCollection<8> collection;
    DarkenFilter(frame);
    DarkenFilter(frame);
    collection.shiftFrames(frame);
    static double alpha = 1.0, alpha_max = 4.0;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            int offset = 0;
            for(int j = 0; j < collection.size(); ++j) {
                cv::Vec3b pix = collection.frames[j].at<cv::Vec3b>(z, i);
                pixel[offset] = static_cast<unsigned char>((pixel[offset]+pix[offset])*alpha);
                
                ++offset;
                if(offset > 2)
                    offset = 0;
            }
            swapColors(frame, z, i);
            if(isNegative) invert(frame, z, i);
        }
    }
    static int dir = 1;
    procPos(dir, alpha, alpha_max, 4.5, 0.01);
}

void ac::RGBTrailsNegativeAlpha(cv::Mat &frame) {
    static MatrixCollection<8> collection;
    DarkenFilter(frame);
    DarkenFilter(frame);
    collection.shiftFrames(frame);
    static double alpha = 1.0, alpha_max = 4.0;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            int offset = 0;
            for(int j = 0; j < collection.size(); ++j) {
                cv::Vec3b pix = collection.frames[j].at<cv::Vec3b>(z, i);
                pixel[offset] = static_cast<unsigned char>((pixel[offset]+~pix[offset])*alpha);
                
                ++offset;
                if(offset > 2)
                    offset = 0;
            }
            swapColors(frame, z, i);
            if(isNegative) invert(frame, z, i);
        }
    }
    static int dir = 1;
    procPos(dir, alpha, alpha_max, 4.5, 0.01);
}

void ac::MovementRGBTrails(cv::Mat &frame) {
    static MatrixCollection<16> collection;
    SmoothRGB(frame, &collection);
}

void ac::RGBTrailsXor(cv::Mat &frame) {
    RGBTrails(frame);
    RandomXorOpposite(frame);
}

void ac::DifferenceStrobe(cv::Mat &frame) {
    static cv::Mat frame_copy = frame.clone();
    cv::Mat orig_frame = frame.clone();
    if(frame_copy.size() != frame.size()) {
        frame_copy = frame.clone();
    }
    static int offset = 0;
    static double alpha = 1.0, alpha_max = 7.0;
    int rand_value = rand()%255;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix = frame_copy.at<cv::Vec3b>(z, i);
            if(pixel[offset]+5 >= pix[offset] && pixel[offset]-5 <= pix[offset]) {
                for(int j = 0; j < 3; ++j) {
                    pixel[j] ^= static_cast<unsigned char>(pixel[j]*alpha)+static_cast<unsigned char>(pix[j]*alpha);
                }
            } else {
                pixel[offset] = rand_value;
            }
            swapColors(frame, z, i);
            if(isNegative) invert(frame, z, i);
        }
    }
    
    ++offset;
    if(offset > 2)
        offset = 0;
    
    static int dir = 1;
    procPos(dir, alpha, alpha_max);
    frame_copy = orig_frame.clone();
}

void ac::BlackAndWhiteDifferenceStrobe(cv::Mat &frame) {
    static cv::Mat frame_copy = frame.clone();
    cv::Mat orig_frame = frame.clone();
    if(frame_copy.size() != frame.size()) {
        frame_copy = frame.clone();
    }
    static int offset = 0;
    static double alpha = 1.0, alpha_max = 7.0;
    static int counter = 0;
    static cv::Vec3b white(255, 255, 255), black(0, 0, 0);
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix = frame_copy.at<cv::Vec3b>(z, i);
            if(pixel[offset]+5 >= pix[offset] && pixel[offset]-5 <= pix[offset]) {
                for(int j = 0; j < 3; ++j) {
                    pixel[j] ^= static_cast<unsigned char>(pixel[j]*alpha)+static_cast<unsigned char>(pix[j]*alpha);
                }
            } else {
                if(counter == 0)
                    pixel = black;
                else
                    pixel = white;
            }
            swapColors(frame, z, i);
            if(isNegative) invert(frame, z, i);
        }
    }
    counter = (counter == 0) ? 1 : 0;
    ++offset;
    if(offset > 2)
        offset = 0;
    static int dir = 1;
    procPos(dir, alpha, alpha_max);
    frame_copy = orig_frame.clone();
}

void ac::DifferenceXor(cv::Mat &frame) {
    static cv::Mat frame_copy = frame.clone();
    cv::Mat orig_frame = frame.clone();
    if(frame_copy.size() != frame.size()) {
        frame_copy = frame.clone();
    }
    static int offset = 0;
    static double alpha = 1.0, alpha_max = 7.0;
    cv::Vec3b pix_color(rand()%255, rand()%255, rand()%255);
    static cv::Vec3b white(255, 255, 255), black(0, 0, 0);
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix = frame_copy.at<cv::Vec3b>(z, i);
            if(pixel[offset]+5 >= pix[offset] && pixel[offset]-5 <= pix[offset]) {
                for(int j = 0; j < 3; ++j) {
                    pixel[j] ^= static_cast<unsigned char>(pixel[j]*alpha)+static_cast<unsigned char>(pix[j]*alpha);
                }
            } else {
                for(int j = 0; j < 3; ++j) {
                    pixel[j] = pixel[j]^pix_color[j];
                }
            }
            swapColors(frame, z, i);
            if(isNegative) invert(frame, z, i);
        }
    }
    ++offset;
    if(offset > 2)
        offset = 0;
    static int dir = 1;
    procPos(dir, alpha, alpha_max);
    frame_copy = orig_frame.clone();
}

void ac::DifferenceRand(cv::Mat &frame) {
    static cv::Mat frame_copy = frame.clone();
    cv::Mat orig_frame = frame.clone();
    if(frame_copy.size() != frame.size()) {
        frame_copy = frame.clone();
    }
    static int offset = 0;
    static double alpha = 1.0, alpha_max = 7.0;
    cv::Vec3b pix_color(rand()%255, rand()%255, rand()%255);
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix = frame_copy.at<cv::Vec3b>(z, i);
            if(pixel[offset]+5 >= pix[offset] && pixel[offset]-5 <= pix[offset]) {
                for(int j = 0; j < 3; ++j) {
                    pixel[j] = pixel[j]^pix_color[j];
                }
            } else {
                for(int j = 0; j < 3; ++j) {
                    pixel[j] ^= static_cast<unsigned char>(pixel[j]*alpha)+static_cast<unsigned char>(pix[j]*alpha);
                }
            }
            swapColors(frame, z, i);
            if(isNegative) invert(frame, z, i);
        }
    }
    ++offset;
    if(offset > 2)
        offset = 0;
    static int dir = 1;
    procPos(dir, alpha, alpha_max);
    frame_copy = orig_frame.clone();
}

void ac::DifferenceBrightStrobe(cv::Mat &frame) {
    static cv::Mat frame_copy = frame.clone();
    cv::Mat orig_frame = frame.clone();
    if(frame_copy.size() != frame.size()) {
        frame_copy = frame.clone();
    }
    static int offset = 0;
    static double alpha = 1.0, alpha_max = 7.0;
    static int sw = 0;
    cv::Vec3b rand_pix(rand()%255, rand()%255, rand()%255);
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix = frame_copy.at<cv::Vec3b>(z, i);
            if(pixel[offset]+5 >= pix[offset] && pixel[offset]-5 <= pix[offset]) {
                for(int j = 0; j < 3; ++j) {
                    if(sw == 0)
                        pixel[j] = ~pixel[j];
                    else
                        pixel[j] ^= static_cast<unsigned char>((pixel[j]^rand_pix[j])*alpha);
                }
            } else {
                for(int j = 0; j < 3; ++j) {
                    if(sw == 1)
                        pixel[j] = ~pixel[j];
                    else
                        pixel[j] ^= static_cast<unsigned char>((pixel[j]^rand_pix[j])*alpha);
                }
            }
            swapColors(frame, z, i);
            if(isNegative) invert(frame, z, i);
        }
    }
    ++offset;
    if(offset > 2)
        offset = 0;
    static int dir = 1;

    ++sw;
    if(sw > 1)
        sw = 0;
    
    procPos(dir, alpha, alpha_max);
    frame_copy = orig_frame.clone();
}

void ac::PsycheTrails(cv::Mat &frame) {
    static MatrixCollection<16> collection;
    DarkenImage(frame, 8);
    collection.shiftFrames(frame);
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            int offset = 0;
            for(int j = 0; j < collection.size(); ++j) {
                cv::Vec3b pix = collection.frames[j].at<cv::Vec3b>(z, i);
                pixel[offset] = static_cast<unsigned char>(pixel[offset]+(pix[offset]));
                ++offset;
                if(offset > 2)
                    offset = 0;
            }
            swapColors(frame, z, i);
            if(isNegative) invert(frame, z, i);
        }
    }
    DifferenceXor(frame);
}

void ac::FourSquare(cv::Mat &frame) {
    static MatrixCollection<4> collection;
    collection.shiftFrames(frame);
    int pos_x = 0;
    int pos_y = 0;
    for(int i = 0; i < collection.size(); ++i) {
        cv::Mat out_frame;
        cv::resize(collection.frames[i], out_frame, cv::Size(frame.cols/2,frame.rows/2));
        copyMat(out_frame, 0,0, frame, pos_x, pos_y, frame.cols/2, frame.rows/2);
        pos_x += frame.cols/2;
        if(pos_x > frame.cols-(frame.cols/2)) {
            pos_x = 0;
            pos_y += frame.rows/2;
        }
    }
    AddInvert(frame);
}

void ac::EightSquare(cv::Mat &frame) {
    static MatrixCollection<8> collection;
    collection.shiftFrames(frame);
    int pos_x = 0;
    int pos_y = 0;
    for(int i = 0; i < collection.size(); ++i) {
        cv::Mat out_frame;
        cv::resize(collection.frames[i], out_frame, cv::Size(frame.cols/4,frame.rows/2));
        copyMat(out_frame, 0,0, frame, pos_x, pos_y, frame.cols/4, frame.rows/2);
        pos_x += frame.cols/4;
        if(pos_x > frame.cols-(frame.cols/4)) {
            pos_x = 0;
            pos_y += frame.rows/2;
        }
    }
    AddInvert(frame);
}

void ac::DiagonalSquare(cv::Mat &frame) {
    int pos_x = 0, pos_y = 0;
    int col_w = (frame.cols/4)-1;
    int col_h = (frame.rows/4)-1;
    cv::Mat copy_frame = frame.clone();
    for(int i = 0; i < 3; ++i) {
    	cv::Mat out_frame;
        cv::resize(copy_frame, out_frame, cv::Size(col_w,col_h));
        if(pos_x >= 0 && pos_x <= frame.cols-col_w && pos_y >= 0 && pos_y <= frame.rows-col_h)
        	copyMat(out_frame, 0, 0, frame, pos_x, pos_y, col_w, col_h);
        pos_x += col_w;
        pos_y += col_h;
    }
    AddInvert(frame);
}

void ac::DiagonalSquareRandom(cv::Mat &frame) {
    static MatrixCollection<4> collection;
    collection.shiftFrames(frame);
    int pos_x = 0, pos_y = 0;
    int col_w = (frame.cols/4)-1;
    int col_h = (frame.rows/4)-1;
    Negate(frame);
    for(int i = 0; i < 4; ++i) {
        cv::Mat &copy_frame = collection.frames[i];
        cv::Mat out_frame;
        cv::resize(copy_frame, out_frame, cv::Size(col_w,col_h));
        int index = 0;
        DrawFunction func = getRandomFilter(index);
        func(out_frame);
        copyMat(out_frame, 0, 0, frame, pos_x, pos_y, col_w, col_h);
        pos_x += col_w;
        pos_y += col_h;
    }
    AddInvert(frame);
}