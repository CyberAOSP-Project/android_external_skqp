/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "Resources.h"
#include "SkCodec.h"
#include "SkColorSpace.h"
#include "Test.h"

static SkStreamAsset* resource(const char path[]) {
    SkString fullPath = GetResourcePath(path);
    return SkStream::NewFromFile(fullPath.c_str());
}

static bool almost_equal(float a, float b) {
    return SkTAbs(a - b) < 0.0001f;
}

DEF_TEST(ColorSpaceParseICCProfile, r) {
    SkAutoTDelete<SkStream> stream(resource("color_wheel_with_profile.png"));
    REPORTER_ASSERT(r, nullptr != stream);

    SkAutoTDelete<SkCodec> codec(SkCodec::NewFromStream(stream.detach()));
    REPORTER_ASSERT(r, nullptr != codec);

    SkColorSpace* colorSpace = codec->getColorSpace();
    REPORTER_ASSERT(r, nullptr != colorSpace);

    // No need to use almost equal here.  The color profile that we have extracted
    // actually has a table of gammas.  And our current implementation guesses 2.2f.
    SkFloat3 gammas = colorSpace->gamma();
    REPORTER_ASSERT(r, 2.2f == gammas.fVec[0]);
    REPORTER_ASSERT(r, 2.2f == gammas.fVec[1]);
    REPORTER_ASSERT(r, 2.2f == gammas.fVec[2]);

    // These nine values were extracted from the color profile in isolation (before
    // we embedded it in the png).  Here we check that we still extract the same values.
    SkFloat3x3 xyz = colorSpace->xyz();
    REPORTER_ASSERT(r, almost_equal(0.436066f, xyz.fMat[0]));
    REPORTER_ASSERT(r, almost_equal(0.222488f, xyz.fMat[1]));
    REPORTER_ASSERT(r, almost_equal(0.013916f, xyz.fMat[2]));
    REPORTER_ASSERT(r, almost_equal(0.385147f, xyz.fMat[3]));
    REPORTER_ASSERT(r, almost_equal(0.716873f, xyz.fMat[4]));
    REPORTER_ASSERT(r, almost_equal(0.0970764f, xyz.fMat[5]));
    REPORTER_ASSERT(r, almost_equal(0.143066f, xyz.fMat[6]));
    REPORTER_ASSERT(r, almost_equal(0.0606079f, xyz.fMat[7]));
    REPORTER_ASSERT(r, almost_equal(0.714096f, xyz.fMat[8]));
}
