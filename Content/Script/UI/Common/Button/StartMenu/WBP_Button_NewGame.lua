--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_Button_NewGame_C
local M = UnLua.Class()

function M:OnInitialized()
    self.Overridden.OnInitialized(self)
end

--function M:Initialize(Initializer)
--end

--function M:PreConstruct(IsDesignTime)
--end

-- function M:Construct()
-- end

--function M:Tick(MyGeometry, InDeltaTime)
--end

function M:BP_OnClicked()
    UE.UKismetSystemLibrary.PrintString(nil, "New Game", true, false, UE.FLinearColor(1, 1, 0, 1), 2)
    local world = self:GetWorld()
    UE.UGameplayStatics.OpenLevel(world, self.GameLevel, true, '')
end

function M:LuaTest(color)
    UE.UKismetSystemLibrary.PrintString(nil, "Hello Lua Test fuck you all", true, false, color, 2)
end

return M
