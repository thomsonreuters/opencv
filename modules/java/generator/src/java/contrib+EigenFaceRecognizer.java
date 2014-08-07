package org.opencv.contrib;

import org.opencv.core.Core;

public class EigenFaceRecognizer extends FaceRecognizer {
    private static native long createEigenFaceRecognizer_0();
    private static native long createEigenFaceRecognizer_1(int num_components);
    private static native long createEigenFaceRecognizer_2(int num_components,double threshold);
    private static native long five_0();

    public EigenFaceRecognizer() {
        super(createEigenFaceRecognizer_0());
    }
    public EigenFaceRecognizer(int num_components) {
        super(createEigenFaceRecognizer_1(num_components));
    }
    public EigenFaceRecognizer(int num_components,double threshold) {
        super(createEigenFaceRecognizer_2(num_components,threshold));
    }
}
