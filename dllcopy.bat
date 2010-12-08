@echo off

echo checking OGRE_HOME = "%OGRE_HOME%"...
if "%OGRE_HOME%"=="" (
    goto VARNOTSET
) else (
    if exist %OGRE_HOME% (
        echo - ok!
    ) else (
        goto DIRINVALID )
)
echo checking OGRE_HOME = "%QTDIR%"...
if "%QTDIR%"=="" (
    goto VARNOTSET
) else (
    if exist %QTDIR% (
        echo - ok!
    ) else (
        goto DIRINVALID )
)
echo checking NEWTON_HOME = "%NEWTON_HOME%"...
if "%NEWTON_HOME%"=="" (
    goto VARNOTSET
) else (
    if exist "%NEWTON_HOME%" (
        echo - ok!
    ) else (
        goto DIRINVALID )
)

echo checking ".\dependencies\ogrenewt\newton20\"...
if exist .\dependencies\ogrenewt\newton20\ (
    echo - ok!
) else (
    goto DIRINVALID )

echo checking ".\dependencies\zlib\"...
if exist .\dependencies\zlib\ (
    echo - ok!
) else (
    goto DIRINVALID )
    
echo checking ".\dependencies\ogg\"...
if exist .\dependencies\ogg\ (
    echo - ok!
) else (
    goto DIRINVALID )

echo checking ".\dependencies\vorbis\"...
if exist .\dependencies\vorbis\ (
    echo - ok!
) else (
    goto DIRINVALID )
echo checking ".\dependencies\CEGUI"...
if exist .\dependencies\CEGUI (
    echo - ok!
) else (
    goto DIRINVALID )    

echo copying files...    
    
echo copy %OGRE_HOME%\bin\release\cg.dll to .\debug\cg.dll 
copy %OGRE_HOME%\bin\release\cg.dll .\debug\cg.dll
echo copy %OGRE_HOME%\bin\debug\OgreMain_d.dll to .\debug\OgreMain_d.dll 
copy %OGRE_HOME%\bin\debug\OgreMain_d.dll .\debug\OgreMain_d.dll
echo copy %OGRE_HOME%\bin\debug\OgrePaging_d.dll to .\debug\OgrePaging_d.dll 
copy %OGRE_HOME%\bin\debug\OgrePaging_d.dll .\debug\OgrePaging_d.dll
echo copy %OGRE_HOME%\bin\debug\OgreProperty_d.dll to .\debug\OgreProperty_d.dll 
copy %OGRE_HOME%\bin\debug\OgreProperty_d.dll .\debug\OgreProperty_d.dll
echo copy %OGRE_HOME%\bin\debug\OgreRTShaderSystem_d.dll to .\debug\OgreRTShaderSystem_d.dll 
copy %OGRE_HOME%\bin\debug\OgreRTShaderSystem_d.dll .\debug\OgreRTShaderSystem_d.dll
echo copy %OGRE_HOME%\bin\debug\OgreTerrain_d.dll to .\debug\OgreTerrain_d.dll 
copy %OGRE_HOME%\bin\debug\OgreTerrain_d.dll .\debug\OgreTerrain_d.dll
echo copy %OGRE_HOME%\bin\debug\OIS_d.dll to .\debug\OIS_d.dll 
copy %OGRE_HOME%\bin\debug\OIS_d.dll .\debug\OIS_d.dll
echo copy %OGRE_HOME%\bin\debug\Plugin_BSPSceneManager_d.dll to .\debug\Plugin_BSPSceneManager_d.dll 
copy %OGRE_HOME%\bin\debug\Plugin_BSPSceneManager_d.dll .\debug\Plugin_BSPSceneManager_d.dll
echo copy %OGRE_HOME%\bin\debug\Plugin_CgProgramManager_d.dll to .\debug\Plugin_CgProgramManager_d.dll 
copy %OGRE_HOME%\bin\debug\Plugin_CgProgramManager_d.dll .\debug\Plugin_CgProgramManager_d.dll
echo copy %OGRE_HOME%\bin\debug\Plugin_OctreeSceneManager_d.dll to .\debug\Plugin_OctreeSceneManager_d.dll 
copy %OGRE_HOME%\bin\debug\Plugin_OctreeSceneManager_d.dll .\debug\Plugin_OctreeSceneManager_d.dll
echo copy %OGRE_HOME%\bin\debug\Plugin_OctreeZone_d.dll to .\debug\Plugin_OctreeZone_d.dll 
copy %OGRE_HOME%\bin\debug\Plugin_OctreeZone_d.dll .\debug\Plugin_OctreeZone_d.dll
echo copy %OGRE_HOME%\bin\debug\Plugin_ParticleFX_d.dll to .\debug\Plugin_ParticleFX_d.dll 
copy %OGRE_HOME%\bin\debug\Plugin_ParticleFX_d.dll .\debug\Plugin_ParticleFX_d.dll
echo copy %OGRE_HOME%\bin\debug\Plugin_PCZSceneManager_d.dll to .\debug\Plugin_PCZSceneManager_d.dll 
copy %OGRE_HOME%\bin\debug\Plugin_PCZSceneManager_d.dll .\debug\Plugin_PCZSceneManager_d.dll
echo copy %OGRE_HOME%\bin\debug\RenderSystem_Direct3D9_d.dll to .\debug\RenderSystem_Direct3D9_d.dll 
copy %OGRE_HOME%\bin\debug\RenderSystem_Direct3D9_d.dll .\debug\RenderSystem_Direct3D9_d.dll
echo copy %OGRE_HOME%\bin\debug\RenderSystem_GL_d.dll to .\debug\RenderSystem_GL_d.dll 
copy %OGRE_HOME%\bin\debug\RenderSystem_GL_d.dll .\debug\RenderSystem_GL_d.dll


echo copy %OGRE_HOME%\bin\release\cg.dll to .\release\cg.dll 
copy %OGRE_HOME%\bin\release\cg.dll .\release\cg.dll
echo copy %OGRE_HOME%\bin\release\OgreMain.dll to .\release\OgreMain.dll 
copy %OGRE_HOME%\bin\release\OgreMain.dll .\release\OgreMain.dll
echo copy %OGRE_HOME%\bin\release\OgrePaging.dll to .\release\OgrePaging.dll 
copy %OGRE_HOME%\bin\release\OgrePaging.dll .\release\OgrePaging.dll
echo copy %OGRE_HOME%\bin\release\OgreProperty.dll to .\release\OgreProperty.dll 
copy %OGRE_HOME%\bin\release\OgreProperty.dll .\release\OgreProperty.dll
echo copy %OGRE_HOME%\bin\release\OgreRTShaderSystem.dll to .\release\OgreRTShaderSystem.dll 
copy %OGRE_HOME%\bin\release\OgreRTShaderSystem.dll .\release\OgreRTShaderSystem.dll
echo copy %OGRE_HOME%\bin\release\OgreTerrain.dll to .\release\OgreTerrain.dll 
copy %OGRE_HOME%\bin\release\OgreTerrain.dll .\release\OgreTerrain.dll
echo copy %OGRE_HOME%\bin\release\OIS.dll to .\release\OIS.dll 
copy %OGRE_HOME%\bin\release\OIS.dll .\release\OIS.dll
echo copy %OGRE_HOME%\bin\release\Plugin_BSPSceneManager.dll to .\release\Plugin_BSPSceneManager.dll 
copy %OGRE_HOME%\bin\release\Plugin_BSPSceneManager.dll .\release\Plugin_BSPSceneManager.dll
echo copy %OGRE_HOME%\bin\release\Plugin_CgProgramManager.dll to .\release\Plugin_CgProgramManager.dll 
copy %OGRE_HOME%\bin\release\Plugin_CgProgramManager.dll .\release\Plugin_CgProgramManager.dll
echo copy %OGRE_HOME%\bin\release\Plugin_OctreeSceneManager.dll to .\release\Plugin_OctreeSceneManager.dll 
copy %OGRE_HOME%\bin\release\Plugin_OctreeSceneManager.dll .\release\Plugin_OctreeSceneManager.dll
echo copy %OGRE_HOME%\bin\release\Plugin_OctreeZone.dll to .\release\Plugin_OctreeZone.dll 
copy %OGRE_HOME%\bin\release\Plugin_OctreeZone.dll .\release\Plugin_OctreeZone.dll
echo copy %OGRE_HOME%\bin\release\Plugin_ParticleFX.dll to .\release\Plugin_ParticleFX.dll 
copy %OGRE_HOME%\bin\release\Plugin_ParticleFX.dll .\release\Plugin_ParticleFX.dll
echo copy %OGRE_HOME%\bin\release\Plugin_PCZSceneManager.dll to .\release\Plugin_PCZSceneManager.dll 
copy %OGRE_HOME%\bin\release\Plugin_PCZSceneManager.dll .\release\Plugin_PCZSceneManager.dll
echo copy %OGRE_HOME%\bin\release\RenderSystem_Direct3D9.dll to .\release\RenderSystem_Direct3D9.dll 
copy %OGRE_HOME%\bin\release\RenderSystem_Direct3D9.dll .\release\RenderSystem_Direct3D9.dll
echo copy %OGRE_HOME%\bin\release\RenderSystem_GL.dll to .\release\RenderSystem_GL.dll 
copy %OGRE_HOME%\bin\release\RenderSystem_GL.dll .\release\RenderSystem_GL.dll


echo copy "%NEWTON_HOME%\sdk\x32\dll_vs9\newton.dll" to .\debug\newton.dll 
copy "%NEWTON_HOME%\sdk\x32\dll_vs9\newton.dll" .\debug\newton.dll
echo copy "%NEWTON_HOME%\sdk\x32\dJointLibrary_d.dll" to .\debug\dJointLibrary_d.dll 
copy "%NEWTON_HOME%\sdk\x32\dJointLibrary_d.dll" .\debug\dJointLibrary_d.dll
echo copy "%NEWTON_HOME%\sdk\x32\dll_vs9\newton.dll" to .\release\newton.dll 
copy "%NEWTON_HOME%\sdk\x32\dll_vs9\newton.dll" .\release\newton.dll
echo copy "%NEWTON_HOME%\sdk\x32\dJointLibrary.dll" to .\release\dJointLibrary.dll 
copy "%NEWTON_HOME%\sdk\x32\dJointLibrary.dll" .\release\dJointLibrary.dll


echo copy .\dependencies\ogrenewt\newton20\lib\OgreNewt_d.dll to .\debug\OgreNewt_d.dll 
copy .\dependencies\ogrenewt\newton20\lib\OgreNewt_d.dll .\debug\OgreNewt_d.dll
echo copy .\dependencies\ogrenewt\newton20\lib\OgreNewt.dll to .\release\OgreNewt.dll 
copy .\dependencies\ogrenewt\newton20\lib\OgreNewt.dll .\release\OgreNewt.dll


echo copy .\dependencies\zlib\projects\visualc6\Win32_DLL_Debug\zlib1d.dll to .\debug\zlib1d.dll 
copy .\dependencies\zlib\projects\visualc6\Win32_DLL_Debug\zlib1d.dll .\debug\zlib1d.dll
echo copy .\dependencies\zlib\projects\visualc6\Win32_DLL_Release\zlib1.dll to .\release\zlib1.dll 
copy .\dependencies\zlib\projects\visualc6\Win32_DLL_Release\zlib1.dll .\release\zlib1.dll


echo copy .\dependencies\ogg\win32\VS2008\Win32\Debug\libogg.dll to .\debug\libogg.dll 
copy .\dependencies\ogg\win32\VS2008\Win32\Debug\libogg.dll .\debug\libogg.dll
echo copy .\dependencies\ogg\win32\VS2008\Win32\Release\libogg.dll to .\release\libogg.dll 
copy .\dependencies\ogg\win32\VS2008\Win32\Release\libogg.dll .\release\libogg.dll


echo copy .\dependencies\vorbis\win32\VS2008\Win32\Debug\libvorbis.dll to .\debug\libvorbis.dll 
copy .\dependencies\vorbis\win32\VS2008\Win32\Debug\libvorbis.dll .\debug\libvorbis.dll
echo copy .\dependencies\vorbis\win32\VS2008\Win32\Debug\libvorbisfile.dll to .\debug\libvorbisfile.dll 
copy .\dependencies\vorbis\win32\VS2008\Win32\Debug\libvorbisfile.dll .\debug\libvorbisfile.dll
echo copy .\dependencies\vorbis\win32\VS2008\Win32\Release\libvorbis.dll to .\release\libvorbis.dll 
copy .\dependencies\vorbis\win32\VS2008\Win32\Release\libvorbis.dll .\release\libvorbis.dll
echo copy .\dependencies\vorbis\win32\VS2008\Win32\Release\libvorbisfile.dll to .\release\libvorbisfile.dll 
copy .\dependencies\vorbis\win32\VS2008\Win32\Release\libvorbisfile.dll .\release\libvorbisfile.dll


echo copy .\dependencies\CEGUI\bin\CEGUIbase.dll to .\release\CEGUIbase.dll 
copy .\dependencies\CEGUI\bin\CEGUIbase.dll .\release\CEGUIbase.dll
echo copy .\dependencies\CEGUI\bin\CEGUIExpatParser.dll to .\release\CEGUIExpatParser.dll 
copy .\dependencies\CEGUI\bin\CEGUIExpatParser.dll .\release\CEGUIExpatParser.dll
echo copy .\dependencies\CEGUI\bin\CEGUIFalagardWRBase.dll to .\release\CEGUIFalagardWRBase.dll 
copy .\dependencies\CEGUI\bin\CEGUIFalagardWRBase.dll .\release\CEGUIFalagardWRBase.dll
echo copy .\dependencies\CEGUI\bin\CEGUIOgreRenderer.dll to .\release\CEGUIOgreRenderer.dll 
copy .\dependencies\CEGUI\bin\CEGUIOgreRenderer.dll .\release\CEGUIOgreRenderer.dll
echo copy .\dependencies\CEGUI\bin\CEGUISILLYImageCodec.dll to .\release\CEGUISILLYImageCodec.dll
copy .\dependencies\CEGUI\bin\CEGUISILLYImageCodec.dll .\release\CEGUISILLYImageCodec.dll
echo copy .\dependencies\CEGUI\dependencies\bin\SILLY.dll to .\release\SILLY.dll
copy .\dependencies\CEGUI\dependencies\bin\SILLY.dll .\release\SILLY.dll


echo copy .\dependencies\CEGUI\bin\CEGUIbase_d.dll to .\debug\CEGUIbase_d.dll 
copy .\dependencies\CEGUI\bin\CEGUIbase_d.dll .\debug\CEGUIbase_d.dll
echo copy .\dependencies\CEGUI\bin\CEGUIExpatParser_d.dll to .\debug\CEGUIExpatParser_d.dll 
copy .\dependencies\CEGUI\bin\CEGUIExpatParser_d.dll .\debug\CEGUIExpatParser_d.dll
echo copy .\dependencies\CEGUI\bin\CEGUIFalagardWRBase_d.dll to .\debug\CEGUIFalagardWRBase_d.dll 
copy .\dependencies\CEGUI\bin\CEGUIFalagardWRBase_d.dll .\debug\CEGUIFalagardWRBase_d.dll
echo copy .\dependencies\CEGUI\bin\CEGUIOgreRenderer_d.dll to .\debug\CEGUIOgreRenderer_d.dll 
copy .\dependencies\CEGUI\bin\CEGUIOgreRenderer_d.dll .\debug\CEGUIOgreRenderer_d.dll
echo copy .\dependencies\CEGUI\bin\CEGUISILLYImageCodec_d.dll to .\debug\CEGUISILLYImageCodec_d.dll
copy .\dependencies\CEGUI\bin\CEGUISILLYImageCodec_d.dll .\debug\CEGUISILLYImageCodec_d.dll
echo copy .\dependencies\CEGUI\dependencies\bin\SILLY_d.dll to .\debug\SILLY_d.dll
copy .\dependencies\CEGUI\dependencies\bin\SILLY_d.dll .\debug\SILLY_d.dll

REM Qt DLLs...
echo copy %QTDIR%\bin\QtGui4.dll to .\release\QtGui4.dll
copy %QTDIR%\bin\QtGui4.dll .\release\QtGui4.dll
echo copy %QTDIR%\bin\QtCore4.dll to .\release\QtCore4.dll
copy %QTDIR%\bin\QtCore4.dll .\release\QtCore4.dll

echo copy %QTDIR%\bin\QtGuid4.dll to .\debug\QtGuid4.dll
copy %QTDIR%\bin\QtGuid4.dll .\debug\QtGuid4.dll
echo copy %QTDIR%\bin\QtCored4.dll to .\debug\QtCored4.dll
copy %QTDIR%\bin\QtCored4.dll .\debug\QtCored4.dll

goto END

:VARNOTSET
echo - variable not set!
goto END

:DIRINVALID
echo - path invalid!
goto END

:END
pause
