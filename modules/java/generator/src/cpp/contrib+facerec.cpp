// gen_java.py doesn't support the return type of these yet
#define LOG_TAG "org.opencv.contrib"
#include "common.h"
#ifdef HAVE_OPENCV_CONTRIB
#include "opencv2/contrib/contrib.hpp"

using namespace cv;

/// throw java exception
static void throwJavaException(JNIEnv *env, const std::exception *e, const char *method) {
    std::string what = "unknown exception";
    jclass je = 0;
    if(e) {
        std::string exception_type = "std::exception";
        if(dynamic_cast<const cv::Exception*>(e)) {
            exception_type = "cv::Exception";
            je = env->FindClass("org/opencv/core/CvException");
        }
        what = exception_type + ": " + e->what();
    }

    if(!je) je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, what.c_str());
        LOGE("%s caught %s", method, what.c_str());
        (void)method;        // avoid "unused" warning
    }

extern "C" {

// don't worry about the _10 , _1 resolves to a single underscore

JNIEXPORT jlong JNICALL Java_org_opencv_contrib_EigenFaceRecognizer_createEigenFaceRecognizer_10(JNIEnv* env, jclass);
JNIEXPORT jlong JNICALL Java_org_opencv_contrib_EigenFaceRecognizer_createEigenFaceRecognizer_10(JNIEnv* env, jclass) {
    static const char method_name[] = "contrib::createEigenFaceRecognizer_10()";
    try {
        LOGD("%s", method_name);
        cv::Ptr<cv::FaceRecognizer> pfr = cv::createEigenFaceRecognizer();
        pfr.addref();
        return (jlong) pfr.obj;
    } catch(const std::exception &e) {
        throwJavaException(env, &e, method_name);
    } catch (...) {
        throwJavaException(env, 0, method_name);
    }
    return 0;
}


JNIEXPORT jlong JNICALL Java_org_opencv_contrib_EigenFaceRecognizer_createEigenFaceRecognizer_11(JNIEnv* env, jclass, jint num_components);
JNIEXPORT jlong JNICALL Java_org_opencv_contrib_EigenFaceRecognizer_createEigenFaceRecognizer_11(JNIEnv* env, jclass, jint num_components) {
    static const char method_name[] = "contrib::createEigenFaceRecognizer_11()";
    try {
        LOGD("%s", method_name);
        cv::Ptr<cv::FaceRecognizer> pfr = cv::createEigenFaceRecognizer((int)num_components);
        pfr.addref();
        return (jlong) pfr.obj;
    } catch(const std::exception &e) {
        throwJavaException(env, &e, method_name);
    } catch (...) {
        throwJavaException(env, 0, method_name);
    }
    return 0;
}

JNIEXPORT jlong JNICALL Java_org_opencv_contrib_EigenFaceRecognizer_createEigenFaceRecognizer_12(JNIEnv* env, jclass, jint num_components, jdouble threshold);
JNIEXPORT jlong JNICALL Java_org_opencv_contrib_EigenFaceRecognizer_createEigenFaceRecognizer_12(JNIEnv* env, jclass, jint num_components, jdouble threshold) {
    static const char method_name[] = "contrib::createEigenFaceRecognizer_12()";
    try {
        LOGD("%s", method_name);
        cv::Ptr<cv::FaceRecognizer> pfr = cv::createEigenFaceRecognizer((int)num_components,(double)threshold);
        pfr.addref();
        return (jlong) pfr.obj;
    } catch(const std::exception &e) {
        throwJavaException(env, &e, method_name);
    } catch (...) {
        throwJavaException(env, 0, method_name);
    }
    return 0;
}

// don't worry about the _10 , _1 resolves to a single underscore



JNIEXPORT jlong JNICALL Java_org_opencv_contrib_FisherFaceRecognizer_createEigenFaceRecognizer_10(JNIEnv* env, jclass);
JNIEXPORT jlong JNICALL Java_org_opencv_contrib_FisherFaceRecognizer_createEigenFaceRecognizer_10(JNIEnv* env, jclass) {
    static const char method_name[] = "contrib::createFisherFaceRecognizer_10()";
    try {
        LOGD("%s", method_name);
        cv::Ptr<cv::FaceRecognizer> pfr = cv::createFisherFaceRecognizer();
        pfr.addref();
        return (jlong) pfr.obj;
    } catch(const std::exception &e) {
        throwJavaException(env, &e, method_name);
    } catch (...) {
        throwJavaException(env, 0, method_name);
    }
    return 0;
}


JNIEXPORT jlong JNICALL Java_org_opencv_contrib_FisherFaceRecognizer_createEigenFaceRecognizer_11(JNIEnv* env, jclass, jint num_components);
JNIEXPORT jlong JNICALL Java_org_opencv_contrib_FisherFaceRecognizer_createEigenFaceRecognizer_11(JNIEnv* env, jclass, jint num_components) {
    static const char method_name[] = "contrib::createFisherFaceRecognizer_11()";
    try {
        LOGD("%s", method_name);
        cv::Ptr<cv::FaceRecognizer> pfr = cv::createFisherFaceRecognizer((int)num_components);
        pfr.addref();
        return (jlong) pfr.obj;
    } catch(const std::exception &e) {
        throwJavaException(env, &e, method_name);
    } catch (...) {
        throwJavaException(env, 0, method_name);
    }
    return 0;
}

JNIEXPORT jlong JNICALL Java_org_opencv_contrib_FisherFaceRecognizer_createEigenFaceRecognizer_12(JNIEnv* env, jclass, jint num_components, jdouble threshold);
JNIEXPORT jlong JNICALL Java_org_opencv_contrib_FisherFaceRecognizer_createEigenFaceRecognizer_12(JNIEnv* env, jclass, jint num_components, jdouble threshold) {
    static const char method_name[] = "contrib::createFisherFaceRecognizer_12()";
    try {
        LOGD("%s", method_name);
        cv::Ptr<cv::FaceRecognizer> pfr = cv::createFisherFaceRecognizer((int)num_components,(double)threshold);
        pfr.addref();
        return (jlong) pfr.obj;
    } catch(const std::exception &e) {
        throwJavaException(env, &e, method_name);
    } catch (...) {
        throwJavaException(env, 0, method_name);
    }
    return 0;
}

JNIEXPORT jlong JNICALL Java_org_opencv_contrib_LBPHFaceRecognizer_createLBPHFaceRecognizer_10(JNIEnv* env, jclass);
JNIEXPORT jlong JNICALL Java_org_opencv_contrib_LBPHFaceRecognizer_createLBPHFaceRecognizer_10(JNIEnv* env, jclass) {
    static const char method_name[] = "contrib::createLBPHFaceRecognizer_10()";
    try {
        LOGD("%s", method_name);
        cv::Ptr<cv::FaceRecognizer> pfr = cv::createLBPHFaceRecognizer();
        pfr.addref();
        return (jlong) pfr.obj;
    } catch(const std::exception &e) {
        throwJavaException(env, &e, method_name);
    } catch (...) {
        throwJavaException(env, 0, method_name);
    }
    return 0;
}

JNIEXPORT jlong JNICALL Java_org_opencv_contrib_LBPHFaceRecognizer_createLBPHFaceRecognizer_11(JNIEnv* env, jclass, jint radius);
JNIEXPORT jlong JNICALL Java_org_opencv_contrib_LBPHFaceRecognizer_createLBPHFaceRecognizer_11(JNIEnv* env, jclass, jint radius) {
    static const char method_name[] = "contrib::createLBPHFaceRecognizer_11()";
    try {
        LOGD("%s", method_name);
        cv::Ptr<cv::FaceRecognizer> pfr = cv::createLBPHFaceRecognizer((int)radius);
        pfr.addref();
        return (jlong) pfr.obj;
    } catch(const std::exception &e) {
        throwJavaException(env, &e, method_name);
    } catch (...) {
        throwJavaException(env, 0, method_name);
    }
    return 0;
}

JNIEXPORT jlong JNICALL Java_org_opencv_contrib_LBPHFaceRecognizer_createLBPHFaceRecognizer_12(JNIEnv* env, jclass, jint radius, jint neighbours);
JNIEXPORT jlong JNICALL Java_org_opencv_contrib_LBPHFaceRecognizer_createLBPHFaceRecognizer_12(JNIEnv* env, jclass, jint radius, jint neighbours) {
    static const char method_name[] = "contrib::createLBPHFaceRecognizer_12()";
    try {
        LOGD("%s", method_name);
        cv::Ptr<cv::FaceRecognizer> pfr = cv::createLBPHFaceRecognizer((int)radius,(int)neighbours);
        pfr.addref();
        return (jlong) pfr.obj;
    } catch(const std::exception &e) {
        throwJavaException(env, &e, method_name);
    } catch (...) {
        throwJavaException(env, 0, method_name);
    }
    return 0;
}

} // extern "C"

#endif // HAVE_OPENCV_CONTRIB
