// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class BuildingEscapeVer2Target : TargetRules
{
	public BuildingEscapeVer2Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "BuildingEscapeVer2" } );
	}
}
