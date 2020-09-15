// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class ARKDE_UE4BasicEditorTarget : TargetRules
{
	public ARKDE_UE4BasicEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "ARKDE_UE4Basic" } );
	}
}
