package org.opencv.contrib;

import org.opencv.core.Core;

public class LBPHFaceRecognizer extends FaceRecognizer {
    // C++ jlong Java_org_opencv_contrib_LBPHFaceRecognizer_createLBPHFaceRecognizer_10(JNIEnv* env, jclass);
    private static native long createLBPHFaceRecognizer_0();
    private static native long createLBPHFaceRecognizer_1(int radius);
    private static native long createLBPHFaceRecognizer_2(int radius,int neighbours);

    public LBPHFaceRecognizer() {
        super(createLBPHFaceRecognizer_0());
    }
    public LBPHFaceRecognizer(int radius) {
        super(createLBPHFaceRecognizer_1(radius));
    }
    public LBPHFaceRecognizer(int radius,int neighbours) {
        super(createLBPHFaceRecognizer_2(radius,neighbours));
    }
}
