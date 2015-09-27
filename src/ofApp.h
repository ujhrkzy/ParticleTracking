#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "MyCircle.h"

#include "ofxOpenCv.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    //ライブカメラを使用する際には、カメラ入力を準備
    ofVideoGrabber 		vidGrabber;
    ofVideoPlayer 		vidPlayer;
    
    ofxCvColorImage colorImg; //オリジナルのカラー映像
    ofxCvGrayscaleImage grayImage; //グレースケールに変換後
    ofxCvGrayscaleImage grayBg; //キャプチャーした背景画像
    ofxCvGrayscaleImage grayDiff; //現在の画像と背景との差分
    ofxCvContourFinder contourFinder; //輪郭抽出のためのクラス
    
    bool bLearnBakground; //背景画像を登録したか否か
    bool showCvAnalysis; //解析結果を表示するか
    int displayMode;
    
    //輪郭検出時のパラメータ
    bool findHoles;
    bool useApproximation;
    
    //GUI
    ofxPanel gui;
    ofxFloatSlider lifeLength;
    ofxIntSlider threshold; //2値化の際の敷居値
    ofxFloatSlider minBlobSize;
    ofxFloatSlider maxBlobSize;
    ofxFloatSlider numBlobs;
    ofxButton findHolesButton;
    ofxButton useApproximationButton;
    ofxXmlSettings settings;
    
    //パーティクルの動的配列
    vector <MyCircle *> circles;
};

#endif
