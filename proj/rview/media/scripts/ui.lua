
local objViewer = obj( "rboot" )
local panelTop = 0

--  create cameras panel
local objCamerasPanel = obj( "panel_cameras" )
local objCamBtnTpl = obj( "template", objCamerasPanel )
local objAnimSetCameras = obj( "controls", objCamerasPanel )
local objModel = obj( "model:*", "srv.model" )
local objSceneCam = obj( "PerspCamera", objViewer )
local objCamControl = obj( "CamControl", objViewer )
panelTop = get( objCamerasPanel, "top" )

function OnCamera( item )
    local camName = get( item, "text" )
    if camName == "Editor" then
        set( objCamControl, "visible", "true" )
    else
        local objCam = obj( "camera:" .. camName, objModel )
        set( objSceneCam, "pos", get( objCam, "pos" ) )
        set( objSceneCam, "viewdir", get( objCam, "viewdir" ) )
        set( objSceneCam, "viewup",  get( objCam, "viewup" ) )
        set( objSceneCam, "fovx",  get( objCam, "fovx" ) )
        set( objSceneCam, "ortho",  get( objCam, "ortho" ) )
        set( objCamControl, "visible", "false" )
    end
end

function addCameraButton( camName, curTop )
    local objBtn = create( objCamBtnTpl, "btn" .. camName, objAnimSetCameras )
    set( objBtn, "top", curTop )
    set( objBtn, "text", camName )
    set( objBtn, "visible", "true" )
    connect( objBtn, "unpress", "OnCamera" )
end

set( objCamerasPanel, "visible", "false" )
if objModel ~= nil then
    local curTop = get( objCamBtnTpl, "top" )
    local tSections = children( objModel )
    local nCameras = 0
    for i, f in ipairs( tSections ) do
        if get( f, "type" ) == "camera" then 
            addCameraButton( get( f, "name" ), curTop )
            curTop = curTop + 16;
            connect( objBtn, "unpress", "OnCamera" )
            nCameras = nCameras + 1
        end
	end 
    if nCameras > 0 then
        addCameraButton( "Editor", curTop )
        curTop = curTop + 16;
        set( objCamerasPanel, "visible", "true" )
        set( objCamerasPanel, "height", curTop )
        set( objCamerasPanel, "top", panelTop )
        panelTop = panelTop + curTop
    end
end

--  create animation set panel
local objAnimSetPanel       = obj( "panel_animset" )
local objAnmCheckTpl        = obj( "template", objAnimSetPanel )
local objAnimSetControls    = obj( "controls", objAnimSetPanel )
local objAnim               = obj( "modelanim:*", "srv.model" )
local objAnimInst           = obj( "animinstance:*", objViewer )

function OnAnimChanged( item )
    local objChecks = children( objAnimSetControls )
    for i, f in ipairs( objChecks ) do
        set( f, "pressed", "false" )
    end
    set( item, "pressed", "true" )
    local section = get( item, "text" )
    set( objAnimInst, "section", section )
    set( objAnimInst, "paused", "false" )
    call( objAnimInst, "play" )
end

function OnAnimStopped( item )
    set( objAnimInst, "paused", "true" )
end

set( objAnimSetPanel, "visible", "false" )
if objAnim ~= nil then
    local curTop = get( objAnmCheckTpl, "top" )
    local tSections = children( objAnim )
    local nSections = 0
    for i, f in ipairs( tSections ) do
        local objCheck = create( objAnmCheckTpl, "chAnim" .. i, objAnimSetControls )
        set( objCheck, "top", curTop )
        set( objCheck, "text", get( f, "name" ) )
        set( objCheck, "visible", "true" )
        curTop = curTop + 16;
        connect( objCheck, "press", "OnAnimChanged" )
        connect( objCheck, "unpress", "OnAnimStopped" )
        nSections = nSections + 1
	end 
    if nSections > 0 then
        set( objAnimSetPanel, "visible", "true" )
        set( objAnimSetPanel, "height", curTop )
        set( objAnimSetPanel, "top", panelTop )
        panelTop = panelTop + curTop
        call( children( objAnimSetControls )[1], "press" )
    end
end

-- playback panel
local objPlaybackPanel = obj( "panel_playback" )
set( objPlaybackPanel, "top", panelTop )

while true do
    wait( 1.0 )
end
