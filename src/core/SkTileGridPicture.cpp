/*
 * Copyright 2012 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkTileGridPicture.h"

#include "SkPictureStateTree.h"
#include "SkTileGrid.h"


SkBBoxHierarchy* SkTileGridFactory::operator()(int width, int height) const {
    SkASSERT(fInfo.fMargin.width() >= 0);
    SkASSERT(fInfo.fMargin.height() >= 0);
    // Note: SkIRects are non-inclusive of the right() column and bottom() row.
    // For example, an SkIRect at 0,0 with a size of (1,1) will only have
    // content at pixel (0,0) and will report left=0 and right=1, hence the
    // "-1"s below.
    int xTileCount = (width + fInfo.fTileInterval.width() - 1) / fInfo.fTileInterval.width();
    int yTileCount = (height + fInfo.fTileInterval.height() - 1) / fInfo.fTileInterval.height();
    return SkNEW_ARGS(SkTileGrid, (xTileCount, yTileCount, fInfo,
                                    SkTileGridNextDatum<SkPictureStateTree::Draw>));
}

#ifdef SK_SUPPORT_LEGACY_DERIVED_PICTURE_CLASSES

SkTileGridPicture::SkTileGridPicture(int width, int height,
                                     const SkTileGridFactory::TileGridInfo& info) {
    SkASSERT(info.fMargin.width() >= 0);
    SkASSERT(info.fMargin.height() >= 0);
    fInfo = info;
    // Note: SkIRects are non-inclusive of the right() column and bottom() row.
    // For example, an SkIRect at 0,0 with a size of (1,1) will only have
    // content at pixel (0,0) and will report left=0 and right=1, hence the
    // "-1"s below.
    fXTileCount = (width + info.fTileInterval.width() - 1) / info.fTileInterval.width();
    fYTileCount = (height + info.fTileInterval.height() - 1) / info.fTileInterval.height();
}

SkBBoxHierarchy* SkTileGridPicture::createBBoxHierarchy() const {
    return SkNEW_ARGS(SkTileGrid, (fXTileCount, fYTileCount, fInfo,
         SkTileGridNextDatum<SkPictureStateTree::Draw>));
}

#endif
