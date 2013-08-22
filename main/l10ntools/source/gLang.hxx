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
#ifndef GLANG_HXX
#define GLANG_HXX
#include <string>
#include <vector>



/*****************************************************************************
 ***************************   G L A N G . H X X   ***************************
 *****************************************************************************
 * This is the class definition header of the l10n localizer program,
 * all global classes and their interrelations is defined here
 *****************************************************************************/



/*******************   G L O B A L   D E F I N I T I O N   *******************/






/********************   C L A S S   D E F I N I T I O N   ********************/
class l10nMem_impl;
class l10nMem
{
  public:
    l10nMem();
    ~l10nMem();

    typedef enum
    {
      ENTRY_DELETED,
      ENTRY_ADDED,
      ENTRY_CHANGED,
      ENTRY_NORMAL
    } ENTRY_STATE;

    static void setShowVerbose ();
    static void setShowDebug   ();

    static int  showError   (const std::string& sText, int iLineNo = 0);
    static int  showWarning (const std::string& sText, int iLineNo = 0);
    static void showDebug   (const std::string& sText, int iLineNo = 0);
    static void showVerbose (const std::string& sText, int iLineNo = 0);
    bool        isError            ();

    void setModuleName (const std::string& sModuleName);
    void setLanguage   (const std::string& sLanguage,
                        bool               bCreate,
                        bool               bConvert);
    void loadEntryKey  (int                iLineNo,
                        const std::string& sSourceFile,
                        const std::string& sKey,
                        const std::string& sOrgText,
                        const std::string& sText,
                        bool               bIsFuzzy);

    void setSourceKey  (int                iLineNo,
                        const std::string& sFilename,
                        const std::string& sKey,
                        const std::string& sText);

    void save          (const std::string& sTargetDir,
                        bool               bKid,
                        bool               bForce);
    void dumpMem       (const std::string& sTargetDir);

    int  prepareMerge  ();
    bool getMergeLang  (std::string& sLang,
                        std::string& sText);
};



/********************   C L A S S   D E F I N I T I O N   ********************/
class convert_gen
{
  public:
    convert_gen(l10nMem&           cMemory,
                const std::string& sSourceDir,
                const std::string& sTargetDir,
                const std::string& sSourceFile);
    ~convert_gen();

    // do extract/merge
    bool execute(const bool bMerge);

    // ONLY po should implement these functions
    void startSave(const std::string& sLanguage,
                   const std::string& sFile);
    void save(const std::string& sFileName,
              const std::string& sKey,
              const std::string& sENUStext,
              const std::string& sText,
              bool               bFuzzy);
    void endSave();
    static bool checkAccess(std::string& sFile);
    static bool createDir(std::string& sDir, std::string& sFile);
};



/********************   C L A S S   D E F I N I T I O N   ********************/
class handler
{
  public:
    handler();
    ~handler();

    void checkCommandLine(int argc, char *argv[]);
    void run();

  private:
    enum {DO_CONVERT, DO_EXTRACT, DO_MERGE, DO_MERGE_KID, DO_GENERATE} meWorkMode;
    l10nMem                  mcMemory;
    std::string              msModuleName;
    std::string              msPoOutDir;
    std::string              msPoDir;
    std::string              msSourceDir;
    std::string              msTargetDir;
    bool                     mbForceSave;
    std::vector<std::string> mvSourceFiles;
    std::vector<std::string> mvLanguages;

    void runConvert();
    void runExtract();
    void runMerge(bool bKid);
    void runGenerate();

    void showUsage(std::string& sErr);
    void showManual();
    void loadL10MEM();
    void readFileWithSources();
};
#endif
