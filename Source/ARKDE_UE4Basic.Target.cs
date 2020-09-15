// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class ARKDE_UE4BasicTarget : TargetRules
{
	public ARKDE_UE4BasicTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "ARKDE_UE4Basic" } );
	}
}
