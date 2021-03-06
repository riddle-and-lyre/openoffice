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



#ifndef _CONNECTIVITY_JAVA_SQL_BLOB_HXX_
#define	_CONNECTIVITY_JAVA_SQL_BLOB_HXX_

#include "java/lang/Object.hxx"
#include <com/sun/star/sdbc/XBlob.hpp>
#include <cppuhelper/implbase1.hxx>

namespace connectivity
{

	//**************************************************************
	//************ Class: java.sql.SQLWarning
	//**************************************************************
	class java_sql_Blob :	public java_lang_Object,
                            public ::cppu::WeakImplHelper1< ::com::sun::star::sdbc::XBlob>
	{
	protected:
	// statische Daten fuer die Klasse
		static jclass theClass;
		virtual ~java_sql_Blob();
	public:
		virtual jclass getMyClass() const;
		
		// ein Konstruktor, der fuer das Returnen des Objektes benoetigt wird:
		java_sql_Blob( JNIEnv * pEnv, jobject myObj );

		// XBlob
		virtual sal_Int64 SAL_CALL length(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
		virtual ::com::sun::star::uno::Sequence< sal_Int8 > SAL_CALL getBytes( sal_Int64 pos, sal_Int32 length ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
		virtual ::com::sun::star::uno::Reference< ::com::sun::star::io::XInputStream > SAL_CALL getBinaryStream(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
		virtual sal_Int64 SAL_CALL position( const ::com::sun::star::uno::Sequence< sal_Int8 >& pattern, sal_Int64 start ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
		virtual sal_Int64 SAL_CALL positionOfBlob( const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XBlob >& pattern, sal_Int64 start ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
	};
}
#endif // _CONNECTIVITY_JAVA_SQL_BLOB_HXX_

