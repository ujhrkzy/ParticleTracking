#include "ofApp.h"

void ofApp::setup(){
    //画面の基本設定
    ofBackground(0, 0, 0);
    ofEnableAlphaBlending();
    ofSetFrameRate(80);
    
    //カメラから映像を取り込んで表示
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(320,240);
    
    //使用する画像の領域を確保
    colorImg.allocate(320,240);
    grayImage.allocate(320,240);
    grayBg.allocate(320,240);
    grayDiff.allocate(320,240);
    
    //変数の初期化
    bLearnBakground = false;
    showCvAnalysis = true;
    
    //GUIを設定
    findHoles = true;
    useApproximation = true;
    
    settings.loadFile("settings.xml");
    gui.setup();
    gui.add(lifeLength.setup("lifeLength",settings.getValue("settings:lefeLength",10),1,100));
    gui.add(threshold.setup("threshold",settings.getValue("settings:threshold",200),0,400));
    gui.add(minBlobSize.setup("minBlobSize",settings.getValue("settings:minBlobSize",0),0,1));
    gui.add(maxBlobSize.setup("maxBlobSize",settings.getValue("settings:maxBlobSize",1),0,1));
    gui.add(numBlobs.setup("numBlobs",settings.getValue("settings:numBlobs",50),0,100));
    gui.add(findHolesButton.setup("findHolesButton",true));
    gui.add(useApproximationButton.setup("useApproximationButton",true));
}


void ofApp::update(){
    settings.setValue("settings:lefeLength", lifeLength);
    settings.setValue("settings:threshold", threshold);
    settings.setValue("settings:minBlobSize", minBlobSize);
    settings.setValue("settings:maxBlobSize", maxBlobSize);
    settings.setValue("settings:numBlobs", numBlobs);
    settings.saveFile("settings.xml"); //puts settings.xml file in the bin/data folder
    
    //新規フレームの取り込みをリセット
    bool bNewFrame = false;
    
    //カメラ使用の場合はカメラから新規フレーム取り込み
    vidGrabber.update();
    //新規にフレームが切り替わったか判定
    bNewFrame = vidGrabber.isFrameNew();
    
    //フレームが切り替わった際のみ画像を解析
    if (bNewFrame){
        //取り込んだフレームを画像としてキャプチャ
        colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
        //左右反転
        colorImg.mirror(false, true);
        
        //カラーのイメージをグレースケールに変換
        grayImage = colorImg;
        
        //まだ背景画像が記録されていなければ、現在のフレームを背景画像とする
        if (bLearnBakground == true){
            grayBg = grayImage;
            bLearnBakground = false;
        }
        
        //グレースケールのイメージと取り込んだ背景画像との差分を算出
        grayDiff.absDiff(grayBg, grayImage);
        //画像を2値化(白と黒だけに)する
        grayDiff.threshold(threshold);
        //2値化した画像から輪郭を抽出する
        contourFinder.findContours(grayDiff,
                                   minBlobSize * minBlobSize * grayDiff.getWidth() * grayDiff.getHeight(),
                                   maxBlobSize * maxBlobSize * grayDiff.getWidth() * grayDiff.getHeight(),
                                   numBlobs, findHoles, useApproximation);
        
        //検出した解析結果からBlobの中心位置を求め
        //中心位置にパーティクルを追加
        for (int i = 0; i < contourFinder.nBlobs; i++){
            circles.push_back(new MyCircle(ofPoint(contourFinder.blobs[i].centroid.x,
                                                   contourFinder.blobs[i].centroid.y),
                                           5, 0.4, 0.1,lifeLength));
        }
    }
    
    //パーティクル更新
    for(vector <MyCircle *>::iterator it = circles.begin(); it != circles.end();){
        (*it)->update();
        if ((*it)->dead) {
            delete (*it);
            it = circles.erase(it);
        } else {
            ++it;
        }
    }
}

void ofApp::draw(){
    ofFill();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    ofPushMatrix();
    //画面サイズいっぱいに表示されるようリスケール
    glScalef((float)ofGetWidth() / (float)grayDiff.width,
             (float)ofGetHeight() / (float)grayDiff.height,
             1.0f);
    //ビデオ映像表示
    ofSetColor(255, 255, 255, 127);
    if(displayMode == 1){
        colorImg.draw(0, 0);
    }else if(displayMode == 2){
        grayImage.draw(0,0);
    }else if(displayMode == 3){
        grayBg.draw(0,0);
    }else{
        grayDiff.draw(0,0);
    }
    
    //パーティクルを表示
    for(vector <MyCircle *>::iterator it = circles.begin(); it != circles.end(); ++it){
        (*it)->draw();
    }
    ofPopMatrix();
    //GUIを表示
    gui.draw();
}

void ofApp::keyPressed  (int key){
    //キー入力でモード切り替え
    switch (key){
        case 'f':
            //フルスクリーンのon/off
            ofToggleFullscreen();
            break;
        case 'g':
            //Gui表示のon/off
            //gui.toggleDraw();
            break;
        case ' ':
            //背景画像を新規に取り込む
            bLearnBakground = true;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            displayMode = key;
            break;
    }
}

void ofApp::keyReleased(int key){
}

void ofApp::mouseMoved(int x, int y){
}

void ofApp::mouseDragged(int x, int y, int button){
}

void ofApp::mousePressed(int x, int y, int button){
}

void ofApp::mouseReleased(int x, int y, int button){
}

void ofApp::windowResized(int w, int h){
}
