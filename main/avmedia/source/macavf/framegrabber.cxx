/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/

#include "framegrabber.hxx"
#include "player.hxx"

#include <tools/stream.hxx>
#include <vcl/graph.hxx>
#include <vcl/cvtgrf.hxx>
#include <unotools/localfilehelper.hxx>

using namespace ::com::sun::star;

namespace avmedia { namespace macavf {

// ----------------
// - FrameGrabber -
// ----------------

FrameGrabber::FrameGrabber( const uno::Reference< lang::XMultiServiceFactory >& /*rxMgr*/ )
:   mpImageGen( NULL )
{}

// ------------------------------------------------------------------------------

FrameGrabber::~FrameGrabber()
{
    if( mpImageGen )
        CFRelease( mpImageGen );
}

// ------------------------------------------------------------------------------

bool FrameGrabber::create( const ::rtl::OUString& rURL )
{
    // TODO: use AVPlayer's movie directly instead of loading it here?

    NSString* pNSStr = [NSString stringWithCharacters:rURL.getStr() length:rURL.getLength()];
    NSURL* pNSURL = [NSURL URLWithString: [pNSStr stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    AVAsset* pMovie = [AVURLAsset URLAssetWithURL:pNSURL options:nil];
    if( !pMovie )
    {
        OSL_TRACE( "AVGrabber::create() cannot load url=\"%s\"", [pNSStr UTF8String] );
        return false;
    }
    if( [[pMovie tracksWithMediaType:AVMediaTypeVideo] count] == 0)
    {
        OSL_TRACE( "AVGrabber::create() found no video in url=\"%s\"", [pNSStr UTF8String] );
        return false;
    }

    mpImageGen = [AVAssetImageGenerator assetImageGeneratorWithAsset:pMovie];
    CFRetain( mpImageGen );
    return true;
}

// ------------------------------------------------------------------------------

uno::Reference< graphic::XGraphic > SAL_CALL FrameGrabber::grabFrame( double fMediaTime )
    throw (uno::RuntimeException)
{
    uno::Reference< graphic::XGraphic > xRet;
    if( !mpImageGen )
        return xRet;
    OSL_TRACE( "AVPlayer::grabFrame( %.3fsec)", fMediaTime );

    // get the requested image from the movie
    CGImage* pCGImage = [mpImageGen copyCGImageAtTime:CMTimeMakeWithSeconds(fMediaTime,1000) actualTime:NULL error:NULL];

    // convert the image to a TIFF-formatted byte-array
    CFMutableDataRef pCFData = CFDataCreateMutable( kCFAllocatorDefault, 0 );
    CGImageDestination* pCGImgDest = CGImageDestinationCreateWithData( pCFData, kUTTypeTIFF, 1, 0 );
    CGImageDestinationAddImage( pCGImgDest, pCGImage, NULL );
    CGImageDestinationFinalize( pCGImgDest );
    CFRelease( pCGImgDest );
    const long nBitmapLen = CFDataGetLength( pCFData );
    void* pBitmapBytes = (void*)CFDataGetBytePtr( pCFData );

    // convert the image into the return-value type which is a graphic::XGraphic
    SvMemoryStream aMemStm( pBitmapBytes, nBitmapLen, STREAM_READ | STREAM_WRITE );
    Graphic aGraphic;
    if( GraphicConverter::Import( aMemStm, aGraphic, CVT_TIF ) == ERRCODE_NONE )
        xRet = aGraphic.GetXGraphic();

    // clean up resources
    CFRelease( pCFData );
    return xRet;
}

// ------------------------------------------------------------------------------

::rtl::OUString SAL_CALL FrameGrabber::getImplementationName(  )
    throw (uno::RuntimeException)
{
    return ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( AVMEDIA_MACAVF_FRAMEGRABBER_IMPLEMENTATIONNAME ) );
}

// ------------------------------------------------------------------------------

sal_Bool SAL_CALL FrameGrabber::supportsService( const ::rtl::OUString& ServiceName )
    throw (uno::RuntimeException)
{
    return ServiceName.equalsAsciiL( RTL_CONSTASCII_STRINGPARAM ( AVMEDIA_MACAVF_FRAMEGRABBER_SERVICENAME ) );
}

// ------------------------------------------------------------------------------

uno::Sequence< ::rtl::OUString > SAL_CALL FrameGrabber::getSupportedServiceNames(  )
    throw (uno::RuntimeException)
{
    uno::Sequence< ::rtl::OUString > aRet(1);
    aRet[0] = ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM ( AVMEDIA_MACAVF_FRAMEGRABBER_SERVICENAME ) );

    return aRet;
}

} // namespace macavf
} // namespace avmedia

