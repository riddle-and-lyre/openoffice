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


#include <com/sun/star/beans/XProperty.hpp>
#include <com/sun/star/awt/FontWeight.hpp>
#include <com/sun/star/awt/FontUnderline.hpp>
#include <com/sun/star/awt/FontStrikeout.hpp>
#include <com/sun/star/awt/FontSlant.hpp>
#include <com/sun/star/text/XSimpleText.hpp>
#include <vbahelper/vbafontbase.hxx>

using namespace ::ooo::vba;
using namespace ::com::sun::star;

// form controls use other property name as the remaining OOo API
#define VBAFONTBASE_PROPNAME( ascii_normal, ascii_control ) \
    mbFormControl ? rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ascii_control ) ) : rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ascii_normal ) )

VbaFontBase::VbaFontBase(
        const uno::Reference< XHelperInterface >& xParent,
        const uno::Reference< uno::XComponentContext >& xContext,
        const uno::Reference< css::container::XIndexAccess >& xPalette,
        const uno::Reference< beans::XPropertySet >& xPropertySet,
        bool bFormControl ) throw ( uno::RuntimeException ) :
    VbaFontBase_BASE( xParent, xContext ),
    mxFont( xPropertySet, uno::UNO_SET_THROW ),
    mxPalette( xPalette, uno::UNO_SET_THROW ),
    mbFormControl( bFormControl )
{
}

VbaFontBase::~VbaFontBase()
{
}

void SAL_CALL
VbaFontBase::setSuperscript( const uno::Any& aValue ) throw ( uno::RuntimeException )
{
    // not supported in form controls
    if( mbFormControl )
        return;
    
	sal_Bool bValue = sal_False;
	aValue >>= bValue;
	sal_Int16 nValue = NORMAL;
	sal_Int8 nValue2 = NORMALHEIGHT;

    if( bValue )
	{
		nValue = SUPERSCRIPT;
	    nValue2 = SUPERSCRIPTHEIGHT;
	}
	mxFont->setPropertyValue( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "CharEscapement" ) ), ( uno::Any )nValue );
 	mxFont->setPropertyValue( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "CharEscapementHeight" ) ), ( uno::Any )nValue2 );
}

uno::Any SAL_CALL
VbaFontBase::getSuperscript() throw ( uno::RuntimeException )
{
	short nValue = NORMAL;
    // not supported in form controls
    if( !mbFormControl )
	   mxFont->getPropertyValue( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "CharEscapement" ) ) ) >>= nValue;
	return uno::makeAny( ( nValue == SUPERSCRIPT ) );
}

void SAL_CALL
VbaFontBase::setSubscript( const uno::Any& aValue ) throw ( uno::RuntimeException )
{
    // not supported in form controls
    if( mbFormControl )
        return;

	sal_Bool bValue = sal_False;
	aValue >>= bValue;
	sal_Int16 nValue = NORMAL;
	sal_Int8 nValue2 = NORMALHEIGHT;

    if( bValue )
	{
		nValue= SUBSCRIPT;
	    nValue2 = SUBSCRIPTHEIGHT;
	}

 	mxFont->setPropertyValue( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "CharEscapementHeight" ) ), ( uno::Any )nValue2 );
	mxFont->setPropertyValue( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "CharEscapement" ) ), ( uno::Any )nValue );

}

uno::Any SAL_CALL
VbaFontBase::getSubscript() throw ( uno::RuntimeException )
{
	short nValue = NORMAL;
    // not supported in form controls
    if( !mbFormControl )
	   mxFont->getPropertyValue( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "CharEscapement" ) ) ) >>= nValue;
	return uno::makeAny( ( nValue == SUBSCRIPT ) );
}

void SAL_CALL
VbaFontBase::setSize( const uno::Any& aValue ) throw( uno::RuntimeException )
{
    // form controls need a sal_Int16 containing points, other APIs need a float
    uno::Any aVal( aValue );
    if( mbFormControl )
    {
        float fVal = 0.0;
        aVal >>= fVal;
        aVal <<= static_cast< sal_Int16 >( fVal );
    }
	mxFont->setPropertyValue( VBAFONTBASE_PROPNAME( "CharHeight", "FontHeight" ), aVal );
}

uno::Any SAL_CALL
VbaFontBase::getSize() throw ( uno::RuntimeException )
{
    return mxFont->getPropertyValue( VBAFONTBASE_PROPNAME( "CharHeight", "FontHeight" ) );
}

void SAL_CALL
VbaFontBase::setColorIndex( const uno::Any& _colorindex ) throw( uno::RuntimeException )
{
	sal_Int32 nIndex = 0;
	_colorindex >>= nIndex;

	--nIndex; // OOo indices are zero bases

	// setColor expects colors in XL RGB values
	// #FIXME this is daft we convert OO RGB val to XL RGB val and
	// then back again to OO RGB value
	setColor( OORGBToXLRGB(mxPalette->getByIndex( nIndex )) );
}


uno::Any SAL_CALL
VbaFontBase::getColorIndex() throw ( uno::RuntimeException )
{
	sal_Int32 nColor = 0;

	XLRGBToOORGB( getColor() ) >>= nColor;
	sal_Int32 nElems = mxPalette->getCount();
	sal_Int32 nIndex = -1;
	for ( sal_Int32 count=0; count<nElems; ++count )
       	{
		sal_Int32 nPaletteColor = 0;
		mxPalette->getByIndex( count ) >>= nPaletteColor;
		if ( nPaletteColor == nColor )
		{
			nIndex = count + 1; // 1 based
			break;
		}
	}
	return uno::makeAny( nIndex );
}

void SAL_CALL
VbaFontBase::setBold( const uno::Any& aValue ) throw( uno::RuntimeException )
{
	sal_Bool bValue = sal_False;
	aValue >>= bValue;
	double fBoldValue = awt::FontWeight::NORMAL;
	if( bValue )
		fBoldValue = awt::FontWeight::BOLD;
	mxFont->setPropertyValue( VBAFONTBASE_PROPNAME( "CharWeight", "FontWeight" ), uno::Any( fBoldValue ) );

}

uno::Any SAL_CALL
VbaFontBase::getBold() throw ( uno::RuntimeException )
{
	double fValue = 0.0;
	mxFont->getPropertyValue( VBAFONTBASE_PROPNAME( "CharWeight", "FontWeight" ) ) >>= fValue;
	return uno::makeAny( fValue == awt::FontWeight::BOLD );
}

void SAL_CALL
VbaFontBase::setStrikethrough( const uno::Any& aValue ) throw ( uno::RuntimeException )
{
	sal_Bool bValue = sal_False;
	aValue >>= bValue;
	short nValue = awt::FontStrikeout::NONE;
	if( bValue )
		nValue = awt::FontStrikeout::SINGLE;
	mxFont->setPropertyValue( VBAFONTBASE_PROPNAME( "CharStrikeout", "FontStrikeout" ), uno::Any( nValue ) );
}

uno::Any SAL_CALL
VbaFontBase::getStrikethrough() throw ( uno::RuntimeException )
{
	short nValue = 0;
	mxFont->getPropertyValue( VBAFONTBASE_PROPNAME( "CharStrikeout", "FontStrikeout" ) ) >>= nValue;
	return uno::Any( nValue == awt::FontStrikeout::SINGLE );
}

void  SAL_CALL
VbaFontBase::setShadow( const uno::Any& aValue ) throw ( uno::RuntimeException )
{
    if( !mbFormControl )
	   mxFont->setPropertyValue( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "CharShadowed" ) ), aValue );
}

uno::Any SAL_CALL
VbaFontBase::getShadow() throw (uno::RuntimeException)
{
	return mbFormControl ? uno::Any( false ) : mxFont->getPropertyValue( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "CharShadowed" ) ) );
}

void  SAL_CALL
VbaFontBase::setItalic( const uno::Any& aValue ) throw ( uno::RuntimeException )
{
	sal_Bool bValue = sal_False;
	aValue >>= bValue;
	short nValue = awt::FontSlant_NONE;
	if( bValue )
		nValue = awt::FontSlant_ITALIC;
    mxFont->setPropertyValue( VBAFONTBASE_PROPNAME( "CharPosture", "FontSlant" ), uno::Any( nValue ) );
}

uno::Any SAL_CALL
VbaFontBase::getItalic() throw ( uno::RuntimeException )
{
    awt::FontSlant aFS;
	mxFont->getPropertyValue( VBAFONTBASE_PROPNAME( "CharPosture", "FontSlant" ) ) >>= aFS;
	return uno::makeAny( aFS == awt::FontSlant_ITALIC );
}

void  SAL_CALL
VbaFontBase::setName( const uno::Any& aValue ) throw ( uno::RuntimeException )
{
	rtl::OUString sString;
	aValue >>= sString;
	mxFont->setPropertyValue( VBAFONTBASE_PROPNAME( "CharFontName", "FontName" ), aValue );
}

uno::Any SAL_CALL
VbaFontBase::getName() throw ( uno::RuntimeException )
{
	return mxFont->getPropertyValue( VBAFONTBASE_PROPNAME( "CharFontName", "FontName" ) );
}

uno::Any
VbaFontBase::getColor() throw (uno::RuntimeException)
{
	uno::Any aAny;
	aAny = OORGBToXLRGB( mxFont->getPropertyValue( VBAFONTBASE_PROPNAME( "CharColor", "TextColor" ) ) );
	return aAny;
}

void
VbaFontBase::setColor( const uno::Any& _color  ) throw (uno::RuntimeException)
{
	mxFont->setPropertyValue( VBAFONTBASE_PROPNAME( "CharColor", "TextColor" ), XLRGBToOORGB(_color) );
}

void SAL_CALL
VbaFontBase::setUnderline( const uno::Any& /*aValue*/ ) throw ( uno::RuntimeException )
{
/*
	// default
	sal_Int32 nValue = excel::XlUnderlineStyle::xlUnderlineStyleNone;
	aValue >>= nValue;
	switch ( nValue )
	{
// NOTE:: #TODO #FIMXE
// xlUnderlineStyleDoubleAccounting & xlUnderlineStyleSingleAccounting
// don't seem to be supported in Openoffice.
// The import filter converts them to single or double underlines as appropriate
// So, here at the moment we are similarly silently converting
// xlUnderlineStyleSingleAccounting to xlUnderlineStyleSingle.

		case excel::XlUnderlineStyle::xlUnderlineStyleNone:
			nValue = awt::FontUnderline::NONE;
			break;
		case excel::XlUnderlineStyle::xlUnderlineStyleSingle:
		case excel::XlUnderlineStyle::xlUnderlineStyleSingleAccounting:
			nValue = awt::FontUnderline::SINGLE;
			break;
		case excel::XlUnderlineStyle::xlUnderlineStyleDouble:
		case excel::XlUnderlineStyle::xlUnderlineStyleDoubleAccounting:
			nValue = awt::FontUnderline::DOUBLE;
			break;
		default:
			throw uno::RuntimeException( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM("Unknown value for Underline")), uno::Reference< uno::XInterface >() );
	}

	mxFont->setPropertyValue( VBAFONTBASE_PROPNAME( "CharUnderline", "FontUnderline" ), uno::Any( nValue ) );
*/

}

uno::Any SAL_CALL
VbaFontBase::getUnderline() throw ( uno::RuntimeException )
{
	sal_Int32 nValue = awt::FontUnderline::NONE;
	mxFont->getPropertyValue( VBAFONTBASE_PROPNAME( "CharUnderline", "FontUnderline" ) ) >>= nValue;
/*
	switch ( nValue )
	{
		case  awt::FontUnderline::DOUBLE:
			nValue = excel::XlUnderlineStyle::xlUnderlineStyleDouble;
			break;
		case  awt::FontUnderline::SINGLE:
			nValue = excel::XlUnderlineStyle::xlUnderlineStyleSingle;
			break;
		case  awt::FontUnderline::NONE:
			nValue = excel::XlUnderlineStyle::xlUnderlineStyleNone;
			break;
		default:
			throw uno::RuntimeException( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM("Unknown value retrieved for Underline") ), uno::Reference< uno::XInterface >() );

	}
*/
	return uno::makeAny( nValue );
}


