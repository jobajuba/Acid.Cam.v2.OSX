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

#include"ac.h"

// initalize to null
ac::ParticleEmiter::ParticleEmiter() : part(0), w(0), h(0) {}

// clean up after done
ac::ParticleEmiter::~ParticleEmiter() {
    if(part != 0) {
        for(int i = 0; i < w; ++i)
            delete [] part[i];
        delete [] part;
        part = 0;
    }
}

void ac::ParticleEmiter::reset() {
    w = 0;
    h = 0;
}

// set frame pixel values
void ac::ParticleEmiter::set(cv::Mat &frame) {
    if(static_cast<int>(frame.cols) != w || static_cast<int>(frame.rows) != h) {
        if(part != 0) {
            for(int i = 0; i < w; ++i)
                delete [] part[i];
            delete [] part;
        }
        w = frame.cols;
        h = frame.rows;
        part = new Particle*[w];
        for(int i = 0; i < w; ++i) {
            part[i] = new Particle[h];
            for(int z = 0; z < h; ++z) {
                part[i][z].x = i;
                part[i][z].y = z;
                part[i][z].dir = rand()%4;
            }
        }
    }
    for(int z = 0; z < h; ++z) {
        for(int i = 0; i < w; ++i) {
            cv::Vec3b pixel = frame.at<cv::Vec3b>(z, i);
            part[i][z].pixel = pixel;
        }
    }
}
// draw pixel values to frame
void ac::ParticleEmiter::draw(cv::Mat &frame) {
    movePixels();//move values before drawing
    for(int z = 0; z < h; ++z) {
        for(int i = 0; i < w; ++i) {
            int x_pos = part[i][z].x;
            int y_pos = part[i][z].y;
            if(x_pos > 0 && x_pos < frame.cols && y_pos > 0 && y_pos < frame.rows) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(y_pos, x_pos);
                pixel = part[i][z].pixel;
            }
        }
    }
}
// move pixel coordinates around
void ac::ParticleEmiter::movePixels() {
    for(int i = 0; i < w; ++i) {
        for(int z = 0; z < h; ++z) {
            Particle &p = part[i][z];
            p.m_count ++;
            if(p.m_count > 250) {
                p.m_count = 0;
                p.dir = rand()%4;
                continue;
            }
            switch(p.dir) {
                case DIR_UP:
                    if(p.y > 0) {
                        p.y--;
                    } else {
                        p.y = 1+rand()%(h-1);
                        p.dir = rand()%4;
                    }
                    break;
                case DIR_DOWN:
                    if(p.y < h-1) {
                        p.y++;
                    } else {
                        p.dir = rand()%4;
                        p.y = 1+rand()%(h-1);
                    }
                    break;
                case DIR_LEFT:
                    if(p.x > 0) {
                        p.x--;
                    } else {
                        p.dir = rand()%4;
                        p.x = 1+rand()%(w-1);
                    }
                    break;
                case DIR_RIGHT:
                    if(p.x < w-1) {
                        p.x++;
                    } else {
                        p.dir = rand()%4;
                        p.x = rand()%(w-1);
                    }
                    break;
                default:
                    p.dir = rand()%4;
            }
        }
    }
}

ac::ParticleEmiter emiter; // initialize

// Particle Filter
void ac::ParticleRelease(cv::Mat &frame) {
    emiter.set(frame);// set values each frame
    emiter.draw(frame); // draw values each frame
}
