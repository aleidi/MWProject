--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

---@type WBP_CharacterInfo_Panel_C
local M = UnLua.Class()

--function M:Initialize(Initializer)
--end

function M:PreConstruct(IsDesignTime)
    if IsDesignTime then
        self:Initialize()
    end
end

function M:Construct()
    self:Initialize()
    self:BindEvents()
end

function M:Initialize()
    if self.UIWidget==nil then
        self.UIWidget = UE.UClass.Load("/Game/UI/InGame/GameInfo/Character/WBP_CharacterInfo.WBP_CharacterInfo_C")
    end

    if self.UIWidget==nil then
        return
    end

    for i = 0, 2 do
        local widget = NewObject(self.UIWidget, self)
        if widget then
            local slot = self.Container:AddChildToVerticalBox(widget)
            if slot then
                --slot:SetSize(UE.FSlateChildSize(1.0, UE.ESlateSizeRule.Automatic))
                slot:SetPadding(UE.FMargin(0.0, 5.0, 0.0, 5.0))
            end
        end
    end
end

function M:BindEvents()
    local world = self:GetWorld()
    if world then
        local mwbs = UE.UWMGameplayUtility.GetBattleSubSystem(world)
        if mwbs then
            mwbs.OnPartyUpdate:Add(self, M.OnCharacterInfoUpdate)
        end
    end
end

---@param Info FMWCharacterInfo
function M:OnCharacterInfoUpdate(Info)
    -- empty container
    self.Container:ClearChildren()
    -- add new info
    if Info.CharacterNames:Length()>0 then
        for i = 1, Info.CharacterNames:Length() do
            local info = self:FindTableRowFromName(Info.CharacterNames:Get(i))
            if info then
                -- UE.UKismetSystemLibrary.PrintString(nil, string.format("id : %d", info.ID), true, false, UE.FLinearColor(1, 1, 0, 1), 10)
                -- UE.UKismetSystemLibrary.PrintString(nil, string.format("name : %s", info.Name), true, false, UE.FLinearColor(1, 1, 0, 1), 10)
                -- UE.UKismetSystemLibrary.PrintString(nil, string.format("tex : %s", info.Head), true, false, UE.FLinearColor(1, 1, 0, 1), 10)
                self:AddInfo(info)
            end
        end
    end
end

function M:FindTableRowFromName(RowName)
    if self.mwgs==nil then
        local world = self:GetWorld()
        if world then
            self.mwgs = UE.UWMGameplayUtility.GetGameplaySubsystem(world)
        end
    end
    
    if self.mwgs then
        local dt = self.mwgs:GetTableCharacterInfo()
        if dt then
            local row = UE.FMWCharacterInfo()
            UE.UDataTableFunctionLibrary.GetDataTableRowFromName(dt, RowName, row)
            --UE.UKismetSystemLibrary.PrintString(nil, string.format("name : %s", row.Name), true, false, UE.FLinearColor(1, 1, 0, 1), 10)
            return row
        end
    end
end

---@param Info FMWCharacterInfo
function M:AddInfo(NewInfo)
    if self.UIWidget==nil then
        self.UIWidget = UE.UClass.Load("/Game/UI/InGame/GameInfo/Character/WBP_CharacterInfo.WBP_CharacterInfo_C")
    end

    if self.UIWidget==nil then
        return
    end
    
    local widget = UE.UWidgetBlueprintLibrary.Create(self, self.UIWidget)
    if widget then
        widget:SetCharacterInfo(NewInfo)
        local slot = self.Container:AddChildToVerticalBox(widget)
        if slot then
            --slot:SetSize(UE.FSlateChildSize(1.0, UE.ESlateSizeRule.Automatic))
            slot:SetPadding(UE.FMargin(0.0, 5.0, 0.0, 5.0))
        end
    end
end

--function M:Tick(MyGeometry, InDeltaTime)
--end

return M
