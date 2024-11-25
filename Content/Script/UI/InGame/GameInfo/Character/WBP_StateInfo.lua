--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_StateInfo_C
local M = UnLua.Class()

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

function M:Construct()
    local widget = NewObject(UE.UImage, self)
    local img = UE.LoadObject("/Engine/EngineResources/AICON-Red.AICON-Red")
    widget:SetBrushFromTexture(img, true)
    self:AddChildInfo(widget, 0, 0)
    widget = NewObject(UE.UImage, self)
    self:AddChildInfo(widget, 0, 1)
    widget = NewObject(UE.UImage, self)
    self:AddChildInfo(widget, 0, 2)
end

--function M:Tick(MyGeometry, InDeltaTime)
--end

function M:AddChildInfo(ChildWidget, InRow , InColumn)
    self.Container:AddChildToUniformGrid(ChildWidget, InRow, InColumn)
    local slot = UE.UWidgetLayoutLibrary.SlotAsUniformGridSlot(ChildWidget)
    if slot then
        slot:SetHorizontalAlignment(self.ImageHorizontalAlignment)
        slot:SetVerticalAlignment(self.ImageVerticalAlignment)
    end
end

return M
