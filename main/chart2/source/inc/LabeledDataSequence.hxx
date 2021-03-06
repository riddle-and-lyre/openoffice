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


#ifndef CHART2_LABELEDDATASEQUENCE_HXX
#define CHART2_LABELEDDATASEQUENCE_HXX
#include "ServiceMacros.hxx"
#include "MutexContainer.hxx"
#include <comphelper/uno3.hxx>
#include <cppuhelper/implbase4.hxx>

#ifndef _COM_SUN_STAR_CHART2_XLABELEDDATASEQUENCE_HPP_
#include <com/sun/star/chart2/data/XLabeledDataSequence.hpp>
#endif
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/util/XCloneable.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>
#include <com/sun/star/util/XModifyBroadcaster.hpp>

namespace chart
{

namespace impl
{
typedef cppu::WeakImplHelper4<
        ::com::sun::star::chart2::data::XLabeledDataSequence,
        ::com::sun::star::util::XCloneable,
        ::com::sun::star::util::XModifyBroadcaster,
        ::com::sun::star::lang::XServiceInfo >
    LabeledDataSequence_Base;
}

class LabeledDataSequence :
        public MutexContainer,
        public impl::LabeledDataSequence_Base
{
public:
	explicit LabeledDataSequence(
        const ::com::sun::star::uno::Reference<
            ::com::sun::star::uno::XComponentContext > & xContext );
	explicit LabeledDataSequence(
        const ::com::sun::star::uno::Reference<
            ::com::sun::star::chart2::data::XDataSequence > & rValues );
	explicit LabeledDataSequence(
        const ::com::sun::star::uno::Reference<
            ::com::sun::star::chart2::data::XDataSequence > & rValues,
        const ::com::sun::star::uno::Reference<
            ::com::sun::star::chart2::data::XDataSequence > & rLabels );

    virtual ~LabeledDataSequence();

    /// establish methods for factory instatiation
	APPHELPER_SERVICE_FACTORY_HELPER( LabeledDataSequence )
    /// declare XServiceInfo methods
    APPHELPER_XSERVICEINFO_DECL()

protected:
    // ____ XLabeledDataSequence ____
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::chart2::data::XDataSequence > SAL_CALL getValues()
        throw (::com::sun::star::uno::RuntimeException);
    virtual void SAL_CALL setValues(
        const ::com::sun::star::uno::Reference< ::com::sun::star::chart2::data::XDataSequence >& xSequence )
        throw (::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::chart2::data::XDataSequence > SAL_CALL getLabel()
        throw (::com::sun::star::uno::RuntimeException);
    virtual void SAL_CALL setLabel(
        const ::com::sun::star::uno::Reference< ::com::sun::star::chart2::data::XDataSequence >& xSequence )
        throw (::com::sun::star::uno::RuntimeException);

    // ____ XCloneable ____
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::util::XCloneable > SAL_CALL createClone()
        throw (::com::sun::star::uno::RuntimeException);

    // ____ XModifyBroadcaster ____
    virtual void SAL_CALL addModifyListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::util::XModifyListener >& aListener )
        throw (::com::sun::star::uno::RuntimeException);
    virtual void SAL_CALL removeModifyListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::util::XModifyListener >& aListener )
        throw (::com::sun::star::uno::RuntimeException);

private:
    ::com::sun::star::uno::Reference< ::com::sun::star::chart2::data::XDataSequence > m_xData;
    ::com::sun::star::uno::Reference< ::com::sun::star::chart2::data::XDataSequence > m_xLabel;

    ::com::sun::star::uno::Reference< ::com::sun::star::uno::XComponentContext >      m_xContext;
    ::com::sun::star::uno::Reference< ::com::sun::star::util::XModifyListener >       m_xModifyEventForwarder;
};

} //  namespace chart

// CHART2_LABELEDDATASEQUENCE_HXX
#endif
