while not game:IsLoaded() or not game:GetService("CoreGui") or not game:GetService("Players").LocalPlayer do task.wait() end

local Gui = Instance.new("ScreenGui")
local Background = Instance.new("Frame")
local MinimizeGuiButton = Instance.new("TextButton")
local FastTeleportButton = Instance.new("TextButton")
local ExitGuiButton = Instance.new("TextButton")
local TeleportButton = Instance.new("ImageButton")
local PlayerTextBox = Instance.new("TextBox")
local FlingModeButton = Instance.new("ImageButton")

Gui.Name = "Gui"
Gui.ResetOnSpawn = false
Gui.IgnoreGuiInset = true
Gui.ScreenInsets = Enum.ScreenInsets.DeviceSafeInsets
Gui.Parent = game:GetService("CoreGui")

Background.Name = "Background"
Background.Active = true
Background.Draggable = true
Background.Size = UDim2.new(0.00, 220.00, 0.00, 70.00)
Background.Position = UDim2.new(0.41, -61.00, 0.39, -14.00)
Background.BorderSizePixel = 0
Background.ZIndex = 999998
Background.BackgroundColor3 = Color3.new(0.20, 0.20, 0.20)
Background.Parent = Gui

MinimizeGuiButton.Name = "MinimizeGuiButton"
MinimizeGuiButton.TextWrapped = true
MinimizeGuiButton.ZIndex = 999999
MinimizeGuiButton.BorderSizePixel = 3
MinimizeGuiButton.BackgroundColor3 = Color3.new(1.00, 0.65, 0.09)
MinimizeGuiButton.FontFace = Font.new("rbxasset://fonts/families/Nunito.json", Enum.FontWeight.Regular, Enum.FontStyle.Normal)
MinimizeGuiButton.TextSize = 50
MinimizeGuiButton.Size = UDim2.new(0.00, 20.00, 0.00, 20.00)
MinimizeGuiButton.TextColor3 = Color3.new(1.00, 1.00, 1.00)
MinimizeGuiButton.BorderColor3 = Color3.new(0.20, 0.20, 0.20)
MinimizeGuiButton.Text = "-"
MinimizeGuiButton.Position = UDim2.new(0.48, 0.00, -0.29, 0.00)
MinimizeGuiButton.Parent = Background

FastTeleportButton.Name = "FastTeleportButton"
FastTeleportButton.ZIndex = 999999
FastTeleportButton.BorderSizePixel = 3
FastTeleportButton.BackgroundColor3 = Color3.new(1.00, 0.25, 0.26)
FastTeleportButton.FontFace = Font.new("rbxasset://fonts/families/SourceSansPro.json", Enum.FontWeight.Regular, Enum.FontStyle.Normal)
FastTeleportButton.TextSize = 14
FastTeleportButton.Size = UDim2.new(0.00, 20.00, 0.00, 20.00)
FastTeleportButton.TextColor3 = Color3.new(0.00, 0.00, 0.00)
FastTeleportButton.BorderColor3 = Color3.new(0.20, 0.20, 0.20)
FastTeleportButton.Text = ""
FastTeleportButton.Position = UDim2.new(0.69, 0.00, -0.29, 0.00)
FastTeleportButton.Parent = Background

ExitGuiButton.Name = "ExitGuiButton"
ExitGuiButton.TextWrapped = true
ExitGuiButton.ZIndex = 999999
ExitGuiButton.BorderSizePixel = 3
ExitGuiButton.BackgroundColor3 = Color3.new(1.00, 0.00, 0.02)
ExitGuiButton.FontFace = Font.new("rbxasset://fonts/families/Nunito.json", Enum.FontWeight.Bold, Enum.FontStyle.Normal)
ExitGuiButton.TextSize = 25
ExitGuiButton.Size = UDim2.new(0.00, 20.00, 0.00, 20.00)
ExitGuiButton.TextColor3 = Color3.new(1.00, 1.00, 1.00)
ExitGuiButton.BorderColor3 = Color3.new(0.20, 0.20, 0.20)
ExitGuiButton.Text = "X"
ExitGuiButton.Position = UDim2.new(0.89, 0.00, -0.29, 0.00)
ExitGuiButton.Parent = Background

TeleportButton.Name = "TeleportButton"
TeleportButton.ZIndex = 999999
TeleportButton.BorderSizePixel = 3
TeleportButton.ScaleType = Enum.ScaleType.Fit
TeleportButton.BackgroundColor3 = Color3.new(1.00, 1.00, 1.00)
TeleportButton.Image = "http://www.roblox.com/asset/?id=7676225854"
TeleportButton.Size = UDim2.new(0.00, 20.00, 0.00, 20.00)
TeleportButton.BorderColor3 = Color3.new(0.20, 0.20, 0.20)
TeleportButton.Position = UDim2.new(0.79, 0.00, -0.29, 0.00)
TeleportButton.Parent = Background

PlayerTextBox.Name = "PlayerTextBox"
PlayerTextBox.TextWrapped = true
PlayerTextBox.ZIndex = 999999
PlayerTextBox.BorderSizePixel = 3
PlayerTextBox.TextScaled = true
PlayerTextBox.BackgroundColor3 = Color3.new(0.36, 0.36, 0.36)
PlayerTextBox.FontFace = Font.new("rbxasset://fonts/families/JosefinSans.json", Enum.FontWeight.Regular, Enum.FontStyle.Normal)
PlayerTextBox.Size = UDim2.new(0.00, 189.00, 0.00, 47.00)
PlayerTextBox.TextColor3 = Color3.new(1.00, 1.00, 1.00)
PlayerTextBox.BorderColor3 = Color3.new(0.15, 0.15, 0.15)
PlayerTextBox.Text = ""
PlayerTextBox.PlaceholderText = "Player Name"
PlayerTextBox.Position = UDim2.new(0.07, 0.00, 0.16, 0.00)
PlayerTextBox.Parent = Background

FlingModeButton.Name = "FlingModeButton"
FlingModeButton.ZIndex = 999999
FlingModeButton.BorderSizePixel = 3
FlingModeButton.ScaleType = Enum.ScaleType.Fit
FlingModeButton.BackgroundColor3 = Color3.new(0.00, 0.00, 0.00)
FlingModeButton.Image = "http://www.roblox.com/asset/?id=13762187293"
FlingModeButton.Size = UDim2.new(0.00, 20.00, 0.00, 20.00)
FlingModeButton.BorderColor3 = Color3.new(0.20, 0.20, 0.20)
FlingModeButton.Position = UDim2.new(0.58, 0.00, -0.29, 0.00)
FlingModeButton.Parent = Background

local cmdlp = game:GetService("Players").LocalPlayer
local userInputService = game:GetService("UserInputService")

local guiMode = true
local fastTpMode = false
local fastWhileMode = false
local flingMode = false
local minimized = false

local speed = 999999999

function find(plr: string): { Player? }
	local f = {}
	local p = plr:lower()
	for _, v in ipairs(game:GetService("Players"):GetPlayers()) do
		if v.Name:lower():sub(1, #plr) == p then
			table.insert(f, v)
		end
	end
	return f
end

function main(): ()
	local t = PlayerTextBox.Text
	local plr = find(t)[1]
	--[[global]] root = cmdlp.Character:FindFirstChild("HumanoidRootPart")
	
	if plr and plr.Character and plr.Character:FindFirstChild("HumanoidRootPart") and cmdlp.Character and root then
		root.CFrame = plr.Character:FindFirstChild("HumanoidRootPart").CFrame
	end
end

function maid(): ()
	if root then
		for _, v in ipairs(root:GetChildren()) do
			if v:IsA("BodyAngularVelocity") and v.Name == "Rocket" then
				v:Destroy()
			end
		end
	end
end

userInputService.InputBegan:Connect(function(UserInput)
	if UserInput.UserInputType == Enum.UserInputType.Keyboard and UserInput.KeyCode == Enum.KeyCode.Insert then
		if guiMode then
			Gui.Enabled = false
			guiMode = false
		else
			Gui.Enabled = true
			guiMode = true
		end
	end
end)

PlayerTextBox.FocusLost:Connect(function(enter) if enter then main() end end)
ExitGuiButton.MouseButton1Click:Connect(function() Gui:Destroy() end)
TeleportButton.MouseButton1Click:Connect(function() main() end)
MinimizeGuiButton.MouseButton1Click:Connect(function()
	minimized = not minimized
	if guiMode then
		if minimized then
			Background.BackgroundTransparency = 1
			PlayerTextBox.Transparency = 1
			Background.Draggable = false
			Background.Selectable = false
			PlayerTextBox.Active = false
			PlayerTextBox.Selectable = false
			PlayerTextBox.Interactable = false
		else
			Background.BackgroundTransparency = 0
			PlayerTextBox.Transparency = 0
			Background.Draggable = true
			Background.Selectable = true
			PlayerTextBox.Active = true
			PlayerTextBox.Selectable = true
			PlayerTextBox.Interactable = true
		end
	end
end)

FlingModeButton.MouseButton1Click:Connect(function()
	flingMode = not flingMode
	if flingMode then
		FlingModeButton.Image = "http://www.roblox.com/asset/?id=15668330759"
	else
		maid()
		FlingModeButton.Image = "http://www.roblox.com/asset/?id=13762187293"
	end
end)

FastTeleportButton.MouseButton1Click:Connect(function()
	fastTpMode = not fastTpMode
	FastTeleportButton.BackgroundColor3 = fastTpMode and Color3.fromRGB(67, 255, 67) or Color3.fromRGB(255, 64, 67)
	if fastTpMode then
		fastWhileMode = true
		while fastWhileMode do
			if flingMode then
				local Main = Instance.new("BodyAngularVelocity", root)
				Main.Name = "Rocket"
				Main.MaxTorque = Vector3.new(0, math.huge, 0)
				Main.AngularVelocity = Vector3.new(0, speed, 0)
			end
			main()
			task.wait()
		end
	else
		fastWhileMode = false
	end
end)
