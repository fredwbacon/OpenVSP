//
// This file is released under the terms of the NASA Open Source Agreement (NOSA)
// version 1.3 as detailed in the LICENSE file which accompanies this software.
//

// StructScreen.cpp: implementation of the StructScreen class.
//
//////////////////////////////////////////////////////////////////////

#include "StructScreen.h"
#include "FeaMeshMgr.h"
#include "StructureMgr.h"
#include "ManageViewScreen.h"
#include "GraphicEngine.h"
#include "Display.h"
#include "Camera.h"
#include "ParmMgr.h"
#include "StlHelper.h"

using namespace vsp;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StructScreen::StructScreen( ScreenMgr* mgr ) : TabScreen( mgr, 430, 720, "FEA Structure", 196 )
{
    m_FLTK_Window->callback( staticCloseCB, this );

    int border = 5;

    Fl_Group* structTab = AddTab( "Structure" );
    Fl_Group* structTabGroup = AddSubGroup( structTab, border );
    Fl_Group* partTab = AddTab( "Part" );
    Fl_Group* partTabGroup = AddSubGroup( partTab, border );
    Fl_Group* matTab = AddTab( "Material" );
    Fl_Group* matTabGroup = AddSubGroup( matTab, border );
    Fl_Group* propTab = AddTab( "Property" );
    Fl_Group* propTabGroup = AddSubGroup( propTab, border );
    Fl_Group* bcTab = AddTab( "BCs" );
    Fl_Group* bcTabGroup = AddSubGroup( bcTab, border );
    Fl_Group* meshTab = AddTab( "Mesh" );
    Fl_Group* meshTabGroup = AddSubGroup( meshTab, border );
    Fl_Group* femTab = AddTab( "FEM" );
    Fl_Group* femTabGroup = AddSubGroup( femTab, border );
    Fl_Group* cadTab = AddTab( "CAD" );
    Fl_Group* cadTabGroup = AddSubGroup( cadTab, border );

    //=== Create Console Area ===//
    m_ConsoleLayout.SetGroupAndScreen( m_FLTK_Window, this );

    int textheight = 100;

    m_ConsoleLayout.AddY( m_ConsoleLayout.GetRemainY()
                        - textheight                            // 100
                        - 4 * m_ConsoleLayout.GetStdHeight()    // 4 * 20
                        - m_ConsoleLayout.GetGapHeight()        // 6
                        - border );                             // 5

    // 196 passed to TabScreen constructor above is this sum plus an additional border.
    // textheight + 3 * m_ConsoleLayout.GetStdHeight() + m_ConsoleLayout.GetGapHeight() + 2 * border
    // This is for the contents of m_BorderConsoleLayout.

    m_ConsoleLayout.AddX( border );

    m_ConsoleLayout.AddSubGroupLayout( m_BorderConsoleLayout, m_ConsoleLayout.GetRemainX() - border,
                                       m_ConsoleLayout.GetRemainY() - border );

    m_ConsoleDisplay = m_BorderConsoleLayout.AddFlTextDisplay( textheight );
    m_ConsoleBuffer = new Fl_Text_Buffer;
    m_ConsoleDisplay->buffer( m_ConsoleBuffer );
    m_FLTK_Window->resizable( m_ConsoleDisplay );

    m_BorderConsoleLayout.AddYGap();

    m_BorderConsoleLayout.SetFitWidthFlag( true );
    m_BorderConsoleLayout.SetSameLineFlag( false );
    m_BorderConsoleLayout.AddChoice( m_CurrFeaMeshChoice, "Structure" );

    m_BorderConsoleLayout.SetSameLineFlag( true );
    m_BorderConsoleLayout.SetFitWidthFlag( false );

    m_BorderConsoleLayout.SetButtonWidth( m_BorderConsoleLayout.GetW() / 2 );
    m_BorderConsoleLayout.SetInputWidth( m_BorderConsoleLayout.GetW() / 2 );

    m_BorderConsoleLayout.AddButton( m_ResetPartDisplayButton, "Reset Part Display" );
    m_BorderConsoleLayout.AddButton( m_ResetMeshDisplayButton, "Reset Mesh Display" );
    m_BorderConsoleLayout.ForceNewLine();

    m_BorderConsoleLayout.AddButton( m_IntersectOnlyButton, "Intersect Only" );
    m_BorderConsoleLayout.AddButton( m_ExportCADButton, "Export CAD" );
    m_BorderConsoleLayout.ForceNewLine();

    m_BorderConsoleLayout.AddButton( m_FeaIntersectMeshButton, "Intersect and Mesh" );
    m_BorderConsoleLayout.AddButton( m_FeaExportFEMButton, "Export FEM" );

    //=== Structures Tab ===//
    structTab->show();

    m_StructureTabLayout.SetGroupAndScreen( structTabGroup, this );

    m_StructureTabLayout.AddDividerBox( "General" );

    m_StructureTabLayout.AddButton( m_WikiLinkButton, "Link to FEA Mesh Wiki Documentation" );

    m_StructureTabLayout.AddYGap();

    m_StructureTabLayout.SetSameLineFlag( true );
    m_StructureTabLayout.SetFitWidthFlag( false );
    m_StructureTabLayout.SetChoiceButtonWidth( m_StructureTabLayout.GetRemainX() / 2 );
    m_StructureTabLayout.SetSliderWidth( m_StructureTabLayout.GetRemainX() / 2 );

    m_StructUnitChoice.AddItem( "SI (kg, m)" );
    m_StructUnitChoice.AddItem( "CGS (g, cm)" );
    m_StructUnitChoice.AddItem( "MPA (tonne, mm)" );
    m_StructUnitChoice.AddItem( "BFT (slug, ft)" );
    string squared( 1, (char) 178 );
    string bin_name = "BIN (lbf sec" + squared + " \/ in, in)";
    m_StructUnitChoice.AddItem( bin_name );
    m_StructureTabLayout.AddChoice( m_StructUnitChoice, "Unit System (Mass, Length)" );

    m_StructureTabLayout.SetSameLineFlag( false );
    m_StructureTabLayout.SetFitWidthFlag( true );

    m_StructureTabLayout.ForceNewLine();
    m_StructureTabLayout.AddYGap();

    m_StructureTabLayout.AddDividerBox( "Structure Selection" );

    // Pointer for the widths of each column in the browser to support resizing
    // Last column width must be 0
    static int struct_col_widths[] = { 172, 151, 86, 0 }; // widths for each column

    int browser_h = 150;
    m_StructureSelectBrowser = m_StructureTabLayout.AddColResizeBrowser( struct_col_widths, 3, browser_h );
    m_StructureSelectBrowser->callback( staticScreenCB, this );

    int buttonwidth = m_StructureTabLayout.GetButtonWidth();

    m_StructureTabLayout.SetChoiceButtonWidth( buttonwidth );
    m_StructureTabLayout.SetSliderWidth( ( m_StructureTabLayout.GetW() - ( 2 * m_StructureTabLayout.GetButtonWidth() ) ) / 2 );

    m_StructureTabLayout.SetSameLineFlag( true );
    m_StructureTabLayout.SetFitWidthFlag( false );

    m_StructureTabLayout.AddChoice( m_GeomChoice, "Geom" );
    m_StructureTabLayout.AddChoice( m_SurfSel, "Surface" );

    m_StructureTabLayout.ForceNewLine();

    m_StructureTabLayout.SetButtonWidth( m_StructureTabLayout.GetRemainX() / 2 );

    m_StructureTabLayout.AddButton( m_AddFeaStructButton, "Add Structure" );
    m_StructureTabLayout.AddButton( m_DelFeaStructButton, "Delete Structure" );

    m_StructureTabLayout.ForceNewLine();
    m_StructureTabLayout.AddYGap();

    m_StructureTabLayout.SetSameLineFlag( false );
    m_StructureTabLayout.SetFitWidthFlag( true );

    m_StructureTabLayout.AddSubGroupLayout( m_StructGroup, m_StructureTabLayout.GetW(), m_StructureTabLayout.GetStdHeight() + m_StructureTabLayout.GetGapHeight() );
    m_StructureTabLayout.AddY( m_StructGroup.GetH() );

    buttonwidth *= 2;

    m_StructGroup.SetButtonWidth( buttonwidth );
    m_StructGroup.SetChoiceButtonWidth( buttonwidth );

    m_StructGroup.AddInput( m_FeaStructNameInput, "Struct Name" );

    m_StructGroup.AddYGap();

    m_StructureTabLayout.AddSubGroupLayout( m_StructGeneralGroup, m_StructureTabLayout.GetW(), m_StructureTabLayout.GetRemainY() );

    m_StructGeneralGroup.AddDividerBox( "Orientation" );

    m_StructGeneralGroup.SetSameLineFlag( true );
    m_StructGeneralGroup.SetFitWidthFlag( false );

    m_StructGeneralGroup.SetButtonWidth( m_StructGeneralGroup.GetRemainX() / 3 );

    m_StructGeneralGroup.AddButton( m_OrientFrontButton, "Front" );
    m_StructGeneralGroup.AddButton( m_OrientTopButton, "Top" );
    m_StructGeneralGroup.AddButton( m_OrientSideButton, "Side" );

    m_StructGeneralGroup.ForceNewLine();

    m_StructureTabLayout.AddSubGroupLayout( m_StructWingGroup, m_StructureTabLayout.GetW(), m_StructureTabLayout.GetRemainY() );

    m_StructWingGroup.AddDividerBox( "Orientation" );

    m_StructWingGroup.AddButton( m_OrientWingButton, "Orient Wing" );

    m_StructWingGroup.ForceNewLine();

    //=== Parts Tab ===//
    m_PartTabLayout.SetGroupAndScreen( partTabGroup, this );

    m_PartTabLayout.AddDividerBox( "FEA Part Selection" );

    int start_x = m_PartTabLayout.GetX();
    int start_y = m_PartTabLayout.GetY();

    m_PartTabLayout.AddSubGroupLayout( m_MovePartButtonLayout, 20, browser_h );
    m_PartTabLayout.AddY( browser_h );

    m_MovePartButtonLayout.SetSameLineFlag( false );
    m_MovePartButtonLayout.SetFitWidthFlag( false );

    m_MovePartButtonLayout.SetStdHeight( 20 );
    m_MovePartButtonLayout.SetButtonWidth( 20 );
    m_MovePartButtonLayout.AddButton( m_MovePrtTopButton, "@2<<" );
    m_MovePartButtonLayout.AddYGap();
    m_MovePartButtonLayout.AddButton( m_MovePrtUpButton, "@2<" );
    m_MovePartButtonLayout.AddY( browser_h - 75 - m_PartTabLayout.GetStdHeight() );
    m_MovePartButtonLayout.AddButton( m_MovePrtDownButton, "@2>" );
    m_MovePartButtonLayout.AddYGap();
    m_MovePartButtonLayout.AddButton( m_MovePrtBotButton, "@2>>" );

    m_PartTabLayout.SetY( start_y );
    m_PartTabLayout.AddX( 20 );
    m_PartTabLayout.SetFitWidthFlag( true );

    m_PartTabLayout.AddSubGroupLayout( m_FeaPartBrowserLayout, m_PartTabLayout.GetRemainX(), browser_h );
    m_PartTabLayout.AddY( browser_h );

    // Pointer for the widths of each column in the browser to support resizing
    // Last column width must be 0
    static int part_col_widths[] = { 86, 65, 43, 86, 43, 86, 0 }; // widths for each column

    m_FeaPartSelectBrowser = m_FeaPartBrowserLayout.AddColResizeBrowser( part_col_widths, 6, browser_h );
    m_FeaPartSelectBrowser->callback( staticScreenCB, this );

    m_PartTabLayout.SetX( start_x );

    m_PartTabLayout.SetSameLineFlag( true );
    m_PartTabLayout.SetFitWidthFlag( false );

    m_PartTabLayout.SetButtonWidth( m_StructGroup.GetRemainX() / 4 );
    m_PartTabLayout.SetChoiceButtonWidth( m_StructGroup.GetRemainX() / 4 );
    m_PartTabLayout.SetSliderWidth( m_StructGroup.GetRemainX() / 4 );

    m_PartTabLayout.AddChoice( m_FeaPartChoice, "Type" );
    m_PartTabLayout.AddButton( m_AddFeaPartButton, "Add Part" );
    m_PartTabLayout.AddButton( m_DelFeaPartButton, "Delete Part" );

    m_PartTabLayout.ForceNewLine();
    m_PartTabLayout.AddYGap();

    //==== General FeaPart Parameters ====//
    m_PartTabLayout.AddSubGroupLayout( m_PartGroup, m_PartTabLayout.GetRemainX(), m_PartTabLayout.GetRemainY() );

    m_PartGroup.SetSameLineFlag( true );
    m_PartGroup.SetFitWidthFlag( false );

    m_PartGroup.SetButtonWidth( m_PartGroup.GetRemainX() / 3 );
    m_PartGroup.SetInputWidth( m_PartGroup.GetRemainX() / 3 );

    m_PartGroup.AddInput( m_FeaPartNameInput, "Part Name" );
    m_PartGroup.AddButton( m_EditFeaPartButton, "Edit Part" );

    m_PartGroup.ForceNewLine();
    m_PartGroup.AddYGap();

    m_PartGroup.SetSameLineFlag( false );
    m_PartGroup.SetFitWidthFlag( true );

    m_PartGroup.AddDividerBox( "General" );

    m_PartGroup.SetSameLineFlag( true );
    m_PartGroup.SetFitWidthFlag( false );

    m_PartGroup.SetButtonWidth( m_PartGroup.GetRemainX() / 2 );

    m_PartGroup.AddButton( m_ShowFeaPartButton, "Show" );
    m_PartGroup.AddButton( m_HideFeaPartButton, "Hide" );

    m_DispFeaPartGroup.Init( this );
    m_DispFeaPartGroup.AddButton( m_HideFeaPartButton.GetFlButton() );
    m_DispFeaPartGroup.AddButton( m_ShowFeaPartButton.GetFlButton() );

    //=== Material Tab ===//
    m_MaterialTabLayout.SetGroupAndScreen( matTabGroup, this );

    m_MaterialTabLayout.AddDividerBox( "Material Selection" );

    m_MaterialTabLayout.AddSubGroupLayout( m_MaterialEditGroup, m_MaterialTabLayout.GetW(), m_MaterialTabLayout.GetRemainY() );

    m_FeaMaterialSelectBrowser = m_MaterialEditGroup.AddFlBrowser( browser_h );
    m_FeaMaterialSelectBrowser->labelfont( 13 );
    m_FeaMaterialSelectBrowser->labelsize( 12 );
    m_FeaMaterialSelectBrowser->callback( staticScreenCB, this );

    m_MaterialEditGroup.SetSameLineFlag( true );
    m_MaterialEditGroup.SetFitWidthFlag( false );

    m_MaterialEditGroup.SetButtonWidth( m_MaterialEditGroup.GetRemainX() / 2 );

    m_MaterialEditGroup.AddButton( m_AddFeaMaterialButton, "Add Material" );
    m_MaterialEditGroup.AddButton( m_DelFeaMaterialButton, "Delete Material" );
    m_MaterialEditGroup.ForceNewLine();

    m_MaterialEditGroup.AddSubGroupLayout( m_MaterialEditSubGroup, m_MaterialEditGroup.GetRemainX(), m_MaterialEditGroup.GetRemainY() );

    m_MaterialEditSubGroup.SetButtonWidth( m_MaterialEditSubGroup.GetRemainX() / 3 );

    m_MaterialEditSubGroup.AddYGap();

    m_MaterialEditSubGroup.AddInput( m_FeaMaterialNameInput, "Material Name" );

    m_MaterialEditSubGroup.AddYGap();

    m_MaterialEditSubGroup.AddChoice( m_FeaMaterialTypeChoice, "Type" );
    m_FeaMaterialTypeChoice.AddItem( "Isotropic", vsp::FEA_ISOTROPIC );
    m_FeaMaterialTypeChoice.AddItem( "Orthotropic", vsp::FEA_ENG_ORTHO );
    m_FeaMaterialTypeChoice.UpdateItems();

    m_MaterialEditSubGroup.AddYGap();

    m_MaterialEditSubGroup.AddDividerBox( "Material Properties" );

    m_MaterialEditSubGroup.SetInputWidth( m_MaterialEditSubGroup.GetW() / 6.0 );
    int button3_w = m_MaterialEditSubGroup.GetW() / 3.0 - m_MaterialEditSubGroup.GetInputWidth();
    m_MaterialEditSubGroup.SetButtonWidth( button3_w );
    int unit_w = button3_w;

    m_MaterialEditSubGroup.SetSameLineFlag( true );

    char rho[16];
    int indx = 0;
    indx += fl_utf8encode( 961, &rho[ indx ] ); // Greek character rho
    rho[ indx ] = 0;

    m_MaterialEditSubGroup.SetFitWidthFlag( true );
    m_MaterialEditSubGroup.AddSlider( m_MatDensitySlider, rho, 1e3, "%5.3g", unit_w );
    m_MaterialEditSubGroup.SetFitWidthFlag( false );
    m_MaterialEditSubGroup.SetButtonWidth( unit_w );
    m_MaterialEditSubGroup.AddButton( m_MatDensityUnit, "" );
    m_MatDensityUnit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_MatDensityUnit.GetFlButton()->labelcolor( FL_BLACK );

    m_MaterialEditSubGroup.AddYGap();
    m_MaterialEditSubGroup.ForceNewLine();
    m_MaterialEditSubGroup.SetButtonWidth( button3_w );

    m_MaterialEditSubGroup.SetFitWidthFlag( true );
    m_MaterialEditSubGroup.AddSlider( m_MatElasticModSlider, "E", 1e4, "%5.3g", unit_w );
    m_MaterialEditSubGroup.SetFitWidthFlag( false );
    m_MaterialEditSubGroup.SetButtonWidth( unit_w );
    m_MaterialEditSubGroup.AddButton( m_MatElasticModUnit, "" );
    m_MatElasticModUnit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_MatElasticModUnit.GetFlButton()->labelcolor( FL_BLACK );

    m_MaterialEditSubGroup.ForceNewLine();
    m_MaterialEditSubGroup.SetButtonWidth( button3_w );

    m_MaterialEditSubGroup.AddInput( m_MatE1Input, "E_1", "%5.3g" );
    m_MaterialEditSubGroup.AddInput( m_MatE2Input, "E_2", "%5.3g" );
    m_MaterialEditSubGroup.AddInput( m_MatE3Input, "E_3", "%5.3g" );

    m_MaterialEditSubGroup.AddYGap();
    m_MaterialEditSubGroup.ForceNewLine();

    m_MaterialEditSubGroup.SetFitWidthFlag( true );
    m_MaterialEditSubGroup.AddSlider( m_MatPoissonSlider, "nu", 1, "%5.3g" ); // unit_w // Reserve unit space
    m_MaterialEditSubGroup.SetFitWidthFlag( false );

    m_MaterialEditSubGroup.ForceNewLine();

    m_MaterialEditSubGroup.AddInput( m_Matnu12Input, "nu_12", "%5.3g", 2.0 * m_MaterialEditSubGroup.GetW() / 3.0 );
    m_MaterialEditSubGroup.AddInput( m_Matnu13Input, "nu_13", "%5.3g", 2.0 * m_MaterialEditSubGroup.GetW() / 3.0 );
    m_MaterialEditSubGroup.AddInput( m_Matnu23Input, "nu_23", "%5.3g", 2.0 * m_MaterialEditSubGroup.GetW() / 3.0 );

    m_MaterialEditSubGroup.AddYGap();
    m_MaterialEditSubGroup.ForceNewLine();

    m_MaterialEditSubGroup.SetFitWidthFlag( true );
    m_MaterialEditSubGroup.AddOutput( m_MatShearModOutput, "G", unit_w );
    m_MaterialEditSubGroup.SetFitWidthFlag( false );
    m_MaterialEditSubGroup.SetButtonWidth( unit_w );
    m_MaterialEditSubGroup.AddButton( m_MatShearModUnit, "" );
    m_MatShearModUnit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_MatShearModUnit.GetFlButton()->labelcolor( FL_BLACK );

    m_MaterialEditSubGroup.ForceNewLine();
    m_MaterialEditSubGroup.SetButtonWidth( button3_w );

    m_MaterialEditSubGroup.AddInput( m_MatG12Input, "G_12", "%5.3g" );
    m_MaterialEditSubGroup.AddInput( m_MatG13Input, "G_13", "%5.3g" );
    m_MaterialEditSubGroup.AddInput( m_MatG23Input, "G_23", "%5.3g" );

    m_MaterialEditSubGroup.AddYGap();
    m_MaterialEditSubGroup.ForceNewLine();

    m_MaterialEditSubGroup.SetFitWidthFlag( true );
    m_MaterialEditSubGroup.AddSlider( m_MatThermalExCoeffSlider, "CTE", 10e-5, "%5.3g", unit_w );
    m_MaterialEditSubGroup.SetFitWidthFlag( false );
    m_MaterialEditSubGroup.SetButtonWidth( unit_w );
    m_MaterialEditSubGroup.AddButton( m_MatThermalExCoeffUnit, "" );
    m_MatThermalExCoeffUnit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_MatThermalExCoeffUnit.GetFlButton()->labelcolor( FL_BLACK );

    m_MaterialEditSubGroup.ForceNewLine();
    m_MaterialEditSubGroup.SetButtonWidth( button3_w );

    m_MaterialEditSubGroup.AddInput( m_MatA1Input, "CTE_1", "%5.3g" );
    m_MaterialEditSubGroup.AddInput( m_MatA2Input, "CTE_2", "%5.3g" );
    m_MaterialEditSubGroup.AddInput( m_MatA3Input, "CTE_3", "%5.3g" );

    //=== Property Tab ===//
    m_PropertyTabLayout.SetGroupAndScreen( propTabGroup, this );

    m_PropertyTabLayout.AddDividerBox( "Property Selection" );

    m_PropertyTabLayout.AddSubGroupLayout( m_PropertyEditGroup, m_PropertyTabLayout.GetW(), m_PropertyTabLayout.GetRemainY() );

    // Pointer for the widths of each column in the browser to support resizing
    // Last column width must be 0
    static int prop_col_widths[] = { 130, 70, 80, 130, 0 }; // widths for each column

    m_FeaPropertySelectBrowser = m_PropertyEditGroup.AddColResizeBrowser( prop_col_widths, 4, browser_h - 20 );
    m_FeaPropertySelectBrowser->callback( staticScreenCB, this );

    m_PropertyEditGroup.SetChoiceButtonWidth( buttonwidth );

    m_FeaPropertyType.AddItem( "Shell Property" );
    m_FeaPropertyType.AddItem( "Beam Property" );

    m_PropertyEditGroup.AddChoice( m_FeaPropertyType, "Type" );

    m_PropertyEditGroup.SetSameLineFlag( true );
    m_PropertyEditGroup.SetFitWidthFlag( false );

    m_PropertyEditGroup.SetButtonWidth( m_PropertyEditGroup.GetRemainX() / 2 );

    m_PropertyEditGroup.AddButton( m_AddFeaPropertyButton, "Add Property" );
    m_PropertyEditGroup.AddButton( m_DelFeaPropertyButton, "Delete Property" );
    m_PropertyEditGroup.ForceNewLine();

    m_PropertyEditGroup.AddSubGroupLayout( m_FeaPropertyCommonGroup, m_PropertyEditGroup.GetRemainX(), m_PropertyEditGroup.GetRemainY() );

    m_FeaPropertyCommonGroup.SetButtonWidth( m_FeaPropertyCommonGroup.GetRemainX() / 3 );

    m_FeaPropertyCommonGroup.AddYGap();

    m_FeaPropertyCommonGroup.AddInput( m_FeaPropertyNameInput, "Property Name" );

    m_FeaPropertyCommonGroup.AddYGap();

    m_FeaPropertyCommonGroup.AddSubGroupLayout( m_FeaPropertyShellGroup, m_FeaPropertyCommonGroup.GetRemainX(), m_FeaPropertyCommonGroup.GetRemainY() );

    m_FeaPropertyShellGroup.AddDividerBox( "Shell Properties" );

    m_FeaPropertyShellGroup.SetButtonWidth( m_FeaPropertyShellGroup.GetRemainX() / 3 );
    m_FeaPropertyShellGroup.SetChoiceButtonWidth( m_FeaPropertyShellGroup.GetRemainX() / 3 );

    m_FeaPropertyShellGroup.AddChoice( m_FeaShellMaterialChoice, "Material" );

    m_FeaPropertyShellGroup.SetSameLineFlag( true );
    m_FeaPropertyShellGroup.SetFitWidthFlag( false );

    m_FeaPropertyShellGroup.SetSliderWidth( m_FeaPropertyShellGroup.GetW() / 3 );
    m_FeaPropertyShellGroup.SetInputWidth( m_FeaPropertyShellGroup.GetW() / 6 );

    m_FeaPropertyShellGroup.AddSlider( m_PropThickSlider, "Thickness", 100.0, "%5.3g" );
    m_FeaPropertyShellGroup.SetButtonWidth( m_FeaPropertyShellGroup.GetRemainX() );
    m_FeaPropertyShellGroup.AddButton( m_PropThickUnit, "" );
    m_PropThickUnit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_PropThickUnit.GetFlButton()->labelcolor( FL_BLACK );

    m_FeaPropertyShellGroup.ForceNewLine();

    m_FeaPropertyCommonGroup.AddSubGroupLayout( m_FeaPropertyBeamGroup, m_FeaPropertyCommonGroup.GetRemainX(), m_FeaPropertyCommonGroup.GetRemainY() );

    m_FeaPropertyBeamGroup.AddDividerBox( "Beam Properties" );

    m_FeaPropertyBeamGroup.SetButtonWidth( m_FeaPropertyBeamGroup.GetRemainX() / 3 );
    m_FeaPropertyBeamGroup.SetChoiceButtonWidth( m_FeaPropertyBeamGroup.GetRemainX() / 3 );

    m_FeaPropertyBeamGroup.AddChoice( m_FeaBeamMaterialChoice, "Material" );

    m_FeaPropertyBeamGroup.SetSameLineFlag( true );
    m_FeaPropertyBeamGroup.SetFitWidthFlag( false );

    m_FeaPropertyBeamGroup.SetSliderWidth( m_FeaPropertyBeamGroup.GetRemainX() / 3 );

    m_FeaBeamXSecChoice.AddItem( "General" );
    m_FeaBeamXSecChoice.AddItem( "Circle" );
    m_FeaBeamXSecChoice.AddItem( "Pipe" );
    m_FeaBeamXSecChoice.AddItem( "I" );
    m_FeaBeamXSecChoice.AddItem( "Rectangle" );
    m_FeaBeamXSecChoice.AddItem( "Box" );
    m_FeaPropertyBeamGroup.AddChoice( m_FeaBeamXSecChoice, "Cross Section Type" );

    m_FeaPropertyBeamGroup.AddButton( m_ShowFeaBeamXSecButton, "Show XSec" );

    m_FeaPropertyBeamGroup.ForceNewLine();

    m_FeaPropertyBeamGroup.AddYGap();

    // General XSec
    m_FeaPropertyBeamGroup.AddSubGroupLayout( m_GenXSecGroup, m_FeaPropertyBeamGroup.GetRemainX(), m_FeaPropertyBeamGroup.GetRemainY() );

    m_GenXSecGroup.AddDividerBox( "General XSec" );

    m_GenXSecGroup.SetSameLineFlag( true );
    m_GenXSecGroup.SetFitWidthFlag( false );

    m_GenXSecGroup.SetButtonWidth( m_GenXSecGroup.GetRemainX() / 3 );
    m_GenXSecGroup.SetSliderWidth( m_GenXSecGroup.GetRemainX() / 3 );
    m_GenXSecGroup.SetInputWidth( m_GenXSecGroup.GetRemainX() / 6 );

    m_GenXSecGroup.AddSlider( m_PropAreaSlider, "Cross-Sect Area", 100.0, "%5.3g" );
    m_GenXSecGroup.SetButtonWidth( m_GenXSecGroup.GetRemainX() );
    m_GenXSecGroup.AddButton( m_PropAreaUnit, "" );
    m_PropAreaUnit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_PropAreaUnit.GetFlButton()->labelcolor( FL_BLACK );

    m_GenXSecGroup.ForceNewLine();
    m_GenXSecGroup.SetButtonWidth( m_GenXSecGroup.GetRemainX() / 3 );

    m_GenXSecGroup.AddSlider( m_PropIzzSlider, "Izz", 100.0, "%5.3g" );
    m_GenXSecGroup.SetButtonWidth( m_GenXSecGroup.GetRemainX() );
    m_GenXSecGroup.AddButton( m_PropIzzUnit, "" );
    m_PropIzzUnit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_PropIzzUnit.GetFlButton()->labelcolor( FL_BLACK );

    m_GenXSecGroup.ForceNewLine();
    m_GenXSecGroup.SetButtonWidth( m_GenXSecGroup.GetRemainX() / 3 );

    m_GenXSecGroup.AddSlider( m_PropIyySlider, "Iyy", 100.0, "%5.3g" );
    m_GenXSecGroup.SetButtonWidth( m_GenXSecGroup.GetRemainX() );
    m_GenXSecGroup.AddButton( m_PropIyyUnit, "" );
    m_PropIyyUnit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_PropIyyUnit.GetFlButton()->labelcolor( FL_BLACK );

    m_GenXSecGroup.ForceNewLine();
    m_GenXSecGroup.SetButtonWidth( m_GenXSecGroup.GetRemainX() / 3 );

    m_GenXSecGroup.AddSlider( m_PropIzySlider, "Izy", 100.0, "%5.3g" );
    m_GenXSecGroup.SetButtonWidth( m_GenXSecGroup.GetRemainX() );
    m_GenXSecGroup.AddButton( m_PropIzyUnit, "" );
    m_PropIzyUnit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_PropIzyUnit.GetFlButton()->labelcolor( FL_BLACK );

    m_GenXSecGroup.ForceNewLine();
    m_GenXSecGroup.SetButtonWidth( m_GenXSecGroup.GetRemainX() / 3 );

    m_GenXSecGroup.AddSlider( m_PropIxxSlider, "Ixx", 100.0, "%5.3g" );
    m_GenXSecGroup.SetButtonWidth( m_GenXSecGroup.GetRemainX() );
    m_GenXSecGroup.AddButton( m_PropIxxUnit, "" );
    m_PropIxxUnit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_PropIxxUnit.GetFlButton()->labelcolor( FL_BLACK );

    m_GenXSecGroup.ForceNewLine();

    // Circle XSec
    m_FeaPropertyBeamGroup.AddSubGroupLayout( m_CircXSecGroup, m_FeaPropertyBeamGroup.GetRemainX(), m_FeaPropertyBeamGroup.GetRemainY() );

    m_CircXSecGroup.AddDividerBox( "Circle XSec" );

    m_CircXSecGroup.SetSameLineFlag( true );
    m_CircXSecGroup.SetFitWidthFlag( false );

    m_CircXSecGroup.SetButtonWidth( m_CircXSecGroup.GetRemainX() / 3 );
    m_CircXSecGroup.SetSliderWidth( m_CircXSecGroup.GetRemainX() / 3 );
    m_CircXSecGroup.SetInputWidth( m_CircXSecGroup.GetRemainX() / 6 );

    m_CircXSecGroup.AddSlider( m_CircDim1Slider, "Radius", 100.0, "%5.3f" );
    m_CircXSecGroup.SetButtonWidth( m_CircXSecGroup.GetRemainX() );
    m_CircXSecGroup.AddButton( m_CircDim1Unit, "" );
    m_CircDim1Unit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_CircDim1Unit.GetFlButton()->labelcolor( FL_BLACK );

    // Pipe/Tube XSec
    m_FeaPropertyBeamGroup.AddSubGroupLayout( m_PipeXSecGroup, m_FeaPropertyBeamGroup.GetRemainX(), m_FeaPropertyBeamGroup.GetRemainY() );

    m_PipeXSecGroup.AddDividerBox( "Pipe XSec" );

    m_PipeXSecGroup.SetSameLineFlag( true );
    m_PipeXSecGroup.SetFitWidthFlag( false );

    m_PipeXSecGroup.SetButtonWidth( m_PipeXSecGroup.GetRemainX() / 3 );
    m_PipeXSecGroup.SetSliderWidth( m_PipeXSecGroup.GetRemainX() / 3 );
    m_PipeXSecGroup.SetInputWidth( m_PipeXSecGroup.GetRemainX() / 6 );

    m_PipeXSecGroup.AddSlider( m_PipeDim1Slider, "R_outer", 100.0, "%5.3f" );
    m_PipeXSecGroup.SetButtonWidth( m_PipeXSecGroup.GetRemainX() );
    m_PipeXSecGroup.AddButton( m_PipeDim1Unit, "" );
    m_PipeDim1Unit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_PipeDim1Unit.GetFlButton()->labelcolor( FL_BLACK );

    m_PipeXSecGroup.ForceNewLine();
    m_PipeXSecGroup.SetButtonWidth( m_PipeXSecGroup.GetW() / 3 );

    m_PipeXSecGroup.AddSlider( m_PipeDim2Slider, "R_inner", 100.0, "%5.3f" );
    m_PipeXSecGroup.SetButtonWidth( m_PipeXSecGroup.GetRemainX() );
    m_PipeXSecGroup.AddButton( m_PipeDim2Unit, "" );
    m_PipeDim2Unit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_PipeDim2Unit.GetFlButton()->labelcolor( FL_BLACK );

    // I XSec
    m_FeaPropertyBeamGroup.AddSubGroupLayout( m_IXSecGroup, m_FeaPropertyBeamGroup.GetRemainX(), m_FeaPropertyBeamGroup.GetRemainY() );

    m_IXSecGroup.AddDividerBox( "I XSec" );

    m_IXSecGroup.SetSameLineFlag( true );
    m_IXSecGroup.SetFitWidthFlag( false );

    m_IXSecGroup.SetButtonWidth( m_IXSecGroup.GetRemainX() / 3 );
    m_IXSecGroup.SetSliderWidth( m_IXSecGroup.GetRemainX() / 3 );
    m_IXSecGroup.SetInputWidth( m_IXSecGroup.GetRemainX() / 6 );

    m_IXSecGroup.AddSlider( m_IDim1Slider, "Dim1", 100.0, "%5.3f" );
    m_IXSecGroup.SetButtonWidth( m_IXSecGroup.GetRemainX() );
    m_IXSecGroup.AddButton( m_IDim1Unit, "" );
    m_IDim1Unit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_IDim1Unit.GetFlButton()->labelcolor( FL_BLACK );

    m_IXSecGroup.ForceNewLine();
    m_IXSecGroup.SetButtonWidth( m_IXSecGroup.GetW() / 3 );

    m_IXSecGroup.AddSlider( m_IDim2Slider, "Dim2", 100.0, "%5.3f" );
    m_IXSecGroup.SetButtonWidth( m_IXSecGroup.GetRemainX() );
    m_IXSecGroup.AddButton( m_IDim2Unit, "" );
    m_IDim2Unit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_IDim2Unit.GetFlButton()->labelcolor( FL_BLACK );

    m_IXSecGroup.ForceNewLine();
    m_IXSecGroup.SetButtonWidth( m_IXSecGroup.GetRemainX() / 3 );

    m_IXSecGroup.AddSlider( m_IDim3Slider, "Dim3", 100.0, "%5.3f" );
    m_IXSecGroup.SetButtonWidth( m_IXSecGroup.GetRemainX() );
    m_IXSecGroup.AddButton( m_IDim3Unit, "" );
    m_IDim3Unit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_IDim3Unit.GetFlButton()->labelcolor( FL_BLACK );

    m_IXSecGroup.ForceNewLine();
    m_IXSecGroup.SetButtonWidth( m_IXSecGroup.GetRemainX() / 3 );

    m_IXSecGroup.AddSlider( m_IDim4Slider, "Dim4", 100.0, "%5.3f" );
    m_IXSecGroup.SetButtonWidth( m_IXSecGroup.GetRemainX() );
    m_IXSecGroup.AddButton( m_IDim4Unit, "" );
    m_IDim4Unit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_IDim4Unit.GetFlButton()->labelcolor( FL_BLACK );

    m_IXSecGroup.ForceNewLine();
    m_IXSecGroup.SetButtonWidth( m_IXSecGroup.GetRemainX() / 3 );

    m_IXSecGroup.AddSlider( m_IDim5Slider, "Dim5", 100.0, "%5.3f" );
    m_IXSecGroup.SetButtonWidth( m_IXSecGroup.GetRemainX() );
    m_IXSecGroup.AddButton( m_IDim5Unit, "" );
    m_IDim5Unit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_IDim5Unit.GetFlButton()->labelcolor( FL_BLACK );

    m_IXSecGroup.ForceNewLine();
    m_IXSecGroup.SetButtonWidth( m_IXSecGroup.GetRemainX() / 3 );

    m_IXSecGroup.AddSlider( m_IDim6Slider, "Dim6", 100.0, "%5.3f" );
    m_IXSecGroup.SetButtonWidth( m_IXSecGroup.GetRemainX() );
    m_IXSecGroup.AddButton( m_IDim6Unit, "" );
    m_IDim6Unit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_IDim6Unit.GetFlButton()->labelcolor( FL_BLACK );

    // Rectangle XSec
    m_FeaPropertyBeamGroup.AddSubGroupLayout( m_RectXSecGroup, m_FeaPropertyBeamGroup.GetRemainX(), m_FeaPropertyBeamGroup.GetRemainY() );

    m_RectXSecGroup.AddDividerBox( "Rectangle XSec" );

    m_RectXSecGroup.SetSameLineFlag( true );
    m_RectXSecGroup.SetFitWidthFlag( false );

    m_RectXSecGroup.SetButtonWidth( m_RectXSecGroup.GetRemainX() / 3 );
    m_RectXSecGroup.SetSliderWidth( m_RectXSecGroup.GetRemainX() / 3 );
    m_RectXSecGroup.SetInputWidth( m_RectXSecGroup.GetRemainX() / 6 );

    m_RectXSecGroup.AddSlider( m_RectDim1Slider, "Width", 100.0, "%5.3f" );
    m_RectXSecGroup.SetButtonWidth( m_RectXSecGroup.GetRemainX() );
    m_RectXSecGroup.AddButton( m_RectDim1Unit, "" );
    m_RectDim1Unit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_RectDim1Unit.GetFlButton()->labelcolor( FL_BLACK );

    m_RectXSecGroup.ForceNewLine();
    m_RectXSecGroup.SetButtonWidth( m_RectXSecGroup.GetRemainX() / 3 );

    m_RectXSecGroup.AddSlider( m_RectDim2Slider, "Height", 100.0, "%5.3f" );
    m_RectXSecGroup.SetButtonWidth( m_RectXSecGroup.GetRemainX() );
    m_RectXSecGroup.AddButton( m_RectDim2Unit, "" );
    m_RectDim2Unit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_RectDim2Unit.GetFlButton()->labelcolor( FL_BLACK );

    // Box XSec
    m_FeaPropertyBeamGroup.AddSubGroupLayout( m_BoxXSecGroup, m_FeaPropertyBeamGroup.GetRemainX(), m_FeaPropertyBeamGroup.GetRemainY() );

    m_BoxXSecGroup.AddDividerBox( "Box XSec" );

    m_BoxXSecGroup.SetSameLineFlag( true );
    m_BoxXSecGroup.SetFitWidthFlag( false );

    m_BoxXSecGroup.SetButtonWidth( m_BoxXSecGroup.GetRemainX() / 3 );
    m_BoxXSecGroup.SetSliderWidth( m_BoxXSecGroup.GetRemainX() / 3 );
    m_BoxXSecGroup.SetInputWidth( m_BoxXSecGroup.GetRemainX() / 6 );

    m_BoxXSecGroup.AddSlider( m_BoxDim1Slider, "Dim1", 100.0, "%5.3f" );
    m_BoxXSecGroup.SetButtonWidth( m_BoxXSecGroup.GetRemainX() );
    m_BoxXSecGroup.AddButton( m_BoxDim1Unit, "" );
    m_BoxDim1Unit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_BoxDim1Unit.GetFlButton()->labelcolor( FL_BLACK );

    m_BoxXSecGroup.ForceNewLine();
    m_BoxXSecGroup.SetButtonWidth( m_BoxXSecGroup.GetRemainX() / 3 );

    m_BoxXSecGroup.AddSlider( m_BoxDim2Slider, "Dim2", 100.0, "%5.3f" );
    m_BoxXSecGroup.SetButtonWidth( m_BoxXSecGroup.GetRemainX() );
    m_BoxXSecGroup.AddButton( m_BoxDim2Unit, "" );
    m_BoxDim2Unit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_BoxDim2Unit.GetFlButton()->labelcolor( FL_BLACK );

    m_BoxXSecGroup.ForceNewLine();
    m_BoxXSecGroup.SetButtonWidth( m_BoxXSecGroup.GetRemainX() / 3 );

    m_BoxXSecGroup.AddSlider( m_BoxDim3Slider, "Dim3", 100.0, "%5.3f" );
    m_BoxXSecGroup.SetButtonWidth( m_BoxXSecGroup.GetRemainX() );
    m_BoxXSecGroup.AddButton( m_BoxDim3Unit, "" );
    m_BoxDim3Unit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_BoxDim3Unit.GetFlButton()->labelcolor( FL_BLACK );

    m_BoxXSecGroup.ForceNewLine();
    m_BoxXSecGroup.SetButtonWidth( m_BoxXSecGroup.GetRemainX() / 3 );

    m_BoxXSecGroup.AddSlider( m_BoxDim4Slider, "Dim4", 100.0, "%5.3f" );
    m_BoxXSecGroup.SetButtonWidth( m_BoxXSecGroup.GetRemainX() );
    m_BoxXSecGroup.AddButton( m_BoxDim4Unit, "" );
    m_BoxDim4Unit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_BoxDim4Unit.GetFlButton()->labelcolor( FL_BLACK );

    //=== Boundary Condition Tab ===//
    m_BCTabLayout.SetGroupAndScreen( bcTabGroup, this );

    m_BCTabLayout.AddDividerBox( "Boundary Condition Selection" );

    m_BCTabLayout.AddSubGroupLayout( m_BCEditGroup, m_BCTabLayout.GetW(), m_BCTabLayout.GetRemainY() );

    // Pointer for the widths of each column in the browser to support resizing
    // Last column width must be 0
    static int bc_col_widths[] = { 20, 20, 20, 20, 20, 20, 430-6*20, 0 }; // widths for each column

    m_FeaBCSelectBrowser = m_BCEditGroup.AddColResizeBrowser( bc_col_widths, 7, browser_h - 20 );
    m_FeaBCSelectBrowser->callback( staticScreenCB, this );


    m_BCEditGroup.SetSameLineFlag( true );
    m_BCEditGroup.SetFitWidthFlag( false );

    m_BCEditGroup.SetButtonWidth( m_BCEditGroup.GetRemainX() / 2 );

    m_BCEditGroup.AddButton( m_AddFeaBCButton, "Add BC" );
    m_BCEditGroup.AddButton( m_DelFeaBCButton, "Delete BC" );
    m_BCEditGroup.ForceNewLine();

    m_BCEditGroup.AddSubGroupLayout( m_FeaBCCommonGroup, m_BCEditGroup.GetRemainX(), m_BCEditGroup.GetRemainY() );

    m_FeaBCCommonGroup.AddYGap();

    m_FeaBCCommonGroup.AddDividerBox( "BC Properties" );

    m_FeaBCCommonGroup.AddChoice( m_FeaBCTypeChoice, "Type" );
    m_FeaBCTypeChoice.AddItem( "Structure", vsp::FEA_BC_STRUCTURE );
    m_FeaBCTypeChoice.AddItem( "Part", vsp::FEA_BC_PART );
    m_FeaBCTypeChoice.AddItem( "Sub Surface", vsp::FEA_BC_SUBSURF );
    m_FeaBCTypeChoice.UpdateItems();

    m_FeaBCCommonGroup.AddYGap();

    m_FeaBCCommonGroup.AddChoice( m_FeaBCPartChoice, "Part" );
    m_FeaBCCommonGroup.AddChoice( m_FeaBCSubSurfChoice, "Sub Surface" );

    m_FeaBCCommonGroup.AddYGap();

    m_FeaBCCommonGroup.AddDividerBox( "Degrees of Freedom" );

    m_FeaBCCommonGroup.AddChoice( m_FeaBCModeChoice, "Mode" );
    m_FeaBCModeChoice.AddItem( "User", vsp::FEA_BCM_USER );
    m_FeaBCModeChoice.AddItem( "All", vsp::FEA_BCM_ALL );
    m_FeaBCModeChoice.AddItem( "Pin", vsp::FEA_BCM_PIN );
    m_FeaBCModeChoice.AddItem( "Symmetric", vsp::FEA_BCM_SYMM );
    m_FeaBCModeChoice.AddItem( "Antisymmetric", vsp::FEA_BCM_ASYMM );
    m_FeaBCModeChoice.UpdateItems();

    m_FeaBCCommonGroup.SetSameLineFlag( true );
    m_FeaBCCommonGroup.SetFitWidthFlag( false );

    m_FeaBCCommonGroup.SetButtonWidth( m_FeaBCCommonGroup.GetW() / 4 );

    m_FeaBCCommonGroup.AddLabel( "Translation:", m_FeaBCCommonGroup.GetButtonWidth() );

    m_FeaBCCommonGroup.AddButton( m_TxButton, "X", 1 << 0 );
    m_FeaBCCommonGroup.AddButton( m_TyButton, "Y", 1 << 1 );
    m_FeaBCCommonGroup.AddButton( m_TzButton, "Z", 1 << 2 );

    m_FeaBCCommonGroup.ForceNewLine();

    m_FeaBCCommonGroup.AddLabel( "Rotation:", m_FeaBCCommonGroup.GetButtonWidth() );
    m_FeaBCCommonGroup.AddButton( m_RxButton, "X", 1 << 3 );
    m_FeaBCCommonGroup.AddButton( m_RyButton, "Y", 1 << 4 );
    m_FeaBCCommonGroup.AddButton( m_RzButton, "Z", 1 << 5 );

    m_FeaBCCommonGroup.ForceNewLine();

    m_FeaBCCommonGroup.SetSameLineFlag( false );
    m_FeaBCCommonGroup.SetFitWidthFlag( true );

    m_FeaBCCommonGroup.AddYGap();
    m_FeaBCCommonGroup.AddDividerBox( "Region" );

    int bw = 50; // [X <=]  Slider button width
    int tw = 15; // Toggle button width
    int gap = 5; // Gap width

    m_FeaBCCommonGroup.SetSameLineFlag( true );

    m_FeaBCCommonGroup.SetFitWidthFlag( false );
    m_FeaBCCommonGroup.SetButtonWidth( tw );
    m_FeaBCCommonGroup.AddButton( m_XGTFlagButton, "" );
    m_FeaBCCommonGroup.SetFitWidthFlag( true );
    m_FeaBCCommonGroup.SetButtonWidth( bw - tw );
    m_FeaBCCommonGroup.AddSlider( m_XGTValSlider, "X >=", 10, "%7.5f", m_FeaBCCommonGroup.GetW() * 0.5 + gap );
    m_FeaBCCommonGroup.AddX( gap );
    m_FeaBCCommonGroup.SetFitWidthFlag( false );
    m_FeaBCCommonGroup.SetButtonWidth( tw );
    m_FeaBCCommonGroup.AddButton( m_XLTFlagButton, "" );
    m_FeaBCCommonGroup.SetFitWidthFlag( true );
    m_FeaBCCommonGroup.SetButtonWidth( bw - tw );
    m_FeaBCCommonGroup.AddSlider( m_XLTValSlider, "X <=", 10, "%7.5f" );
    m_FeaBCCommonGroup.ForceNewLine();

    m_FeaBCCommonGroup.SetFitWidthFlag( false );
    m_FeaBCCommonGroup.SetButtonWidth( tw );
    m_FeaBCCommonGroup.AddButton( m_YGTFlagButton, "" );
    m_FeaBCCommonGroup.SetFitWidthFlag( true );
    m_FeaBCCommonGroup.SetButtonWidth( bw - tw );
    m_FeaBCCommonGroup.AddSlider( m_YGTValSlider, "Y >=", 10, "%7.5f", m_FeaBCCommonGroup.GetW() * 0.5 + gap );
    m_FeaBCCommonGroup.AddX( gap );
    m_FeaBCCommonGroup.SetFitWidthFlag( false );
    m_FeaBCCommonGroup.SetButtonWidth( tw );
    m_FeaBCCommonGroup.AddButton( m_YLTFlagButton, "" );
    m_FeaBCCommonGroup.SetFitWidthFlag( true );
    m_FeaBCCommonGroup.SetButtonWidth( bw - tw );
    m_FeaBCCommonGroup.AddSlider( m_YLTValSlider, "Y <=", 10, "%7.5f" );
    m_FeaBCCommonGroup.ForceNewLine();

    m_FeaBCCommonGroup.SetFitWidthFlag( false );
    m_FeaBCCommonGroup.SetButtonWidth( tw );
    m_FeaBCCommonGroup.AddButton( m_ZGTFlagButton, "" );
    m_FeaBCCommonGroup.SetFitWidthFlag( true );
    m_FeaBCCommonGroup.SetButtonWidth( bw - tw );
    m_FeaBCCommonGroup.AddSlider( m_ZGTValSlider, "Z >=", 10, "%7.5f", m_FeaBCCommonGroup.GetW() * 0.5 + gap );
    m_FeaBCCommonGroup.AddX( gap );
    m_FeaBCCommonGroup.SetFitWidthFlag( false );
    m_FeaBCCommonGroup.SetButtonWidth( tw );
    m_FeaBCCommonGroup.AddButton( m_ZLTFlagButton, "" );
    m_FeaBCCommonGroup.SetFitWidthFlag( true );
    m_FeaBCCommonGroup.SetButtonWidth( bw - tw );
    m_FeaBCCommonGroup.AddSlider( m_ZLTValSlider, "Z <=", 10, "%7.5f" );
    m_FeaBCCommonGroup.ForceNewLine();


    //=== MESH TAB ===//
    m_MeshTabLayout.SetGroupAndScreen( meshTabGroup, this );

    m_MeshTabLayout.AddDividerBox( "Mesh Control" );

    m_MeshTabLayout.AddYGap();

    m_MeshTabLayout.SetButtonWidth( 175 );
    m_MeshTabLayout.AddSlider( m_MaxEdgeLen, "Max Edge Len", 1.0, "%7.5f" );
    m_MeshTabLayout.AddSlider( m_MinEdgeLen, "Min Edge Len", 1.0, "%7.5f" );
    m_MeshTabLayout.AddSlider( m_MaxGap, "Max Gap", 1.0, "%7.5f" );
    m_MeshTabLayout.AddSlider( m_NumCircleSegments, "Num Circle Segments", 100.0, "%7.5f" );
    m_MeshTabLayout.AddSlider( m_GrowthRatio, "Growth Ratio", 2.0, "%7.5f" );

    m_MeshTabLayout.AddYGap();
    m_MeshTabLayout.AddDividerBox( "Geometry Control" );
    m_MeshTabLayout.AddYGap();

    m_MeshTabLayout.AddButton( m_Rig3dGrowthLimit, "Rigorous 3D Growth Limiting" );
    m_MeshTabLayout.AddYGap();
    m_MeshTabLayout.AddSlider( m_RelCurveTolSlider, "Curve Adaptation Tolerance", 0.01, "%7.5f" );
    m_MeshTabLayout.AddYGap();
    m_MeshTabLayout.AddButton( m_HalfMeshButton, "Generate Half Mesh" );
    m_MeshTabLayout.AddYGap();
    m_MeshTabLayout.AddButton( m_ToCubicToggle, "Demote Surfs to Cubic" );
    m_MeshTabLayout.AddSlider( m_ToCubicTolSlider, "Cubic Tolerance", 10, "%5.4g", 0, true );

    m_MeshTabLayout.AddYGap();
    m_MeshTabLayout.AddDividerBox( "Element Type" );
    m_MeshTabLayout.AddYGap();

    m_MeshTabLayout.AddButton( m_ConvertToQuadsToggle, "Convert to Quads" );
    m_MeshTabLayout.AddButton( m_HighOrderElementToggle, "High order Elements" );
    m_MeshTabLayout.AddButton( m_BeamPerElementNormalToggle, "Beam Per-Element Normal Vectors" );

    m_MeshTabLayout.AddYGap();
    m_MeshTabLayout.AddDividerBox( "FEA Index Offsets" );
    m_MeshTabLayout.AddYGap();

    m_MeshTabLayout.AddSlider( m_NodeOffset, "Node Offset", 1e5, " %5.0f" );
    m_MeshTabLayout.AddSlider( m_ElementOffset, "Element Offset", 1e5, " %5.0f" );

    //=== FEM Tab ===//
    m_FemTabLayout.SetGroupAndScreen( femTabGroup, this );

    m_FemTabLayout.AddDividerBox( "FEM Export" );

    m_FemTabLayout.SetFitWidthFlag( false );
    m_FemTabLayout.SetSameLineFlag( true );

    m_FemTabLayout.SetInputWidth( m_FemTabLayout.GetW() - 75 - 55 );

    m_FemTabLayout.SetButtonWidth( 75 );
    m_FemTabLayout.AddButton( m_StlFile, ".stl" );
    m_FemTabLayout.AddOutput( m_StlOutput );
    m_FemTabLayout.SetButtonWidth( m_FemTabLayout.GetRemainX() );
    m_FemTabLayout.AddButton( m_SelectStlFile, "..." );
    m_FemTabLayout.ForceNewLine();

    m_FemTabLayout.SetButtonWidth( 75 );
    m_FemTabLayout.AddButton( m_GmshFile, ".msh" );
    m_FemTabLayout.AddOutput( m_GmshOutput );
    m_FemTabLayout.SetButtonWidth( m_FemTabLayout.GetRemainX() );
    m_FemTabLayout.AddButton( m_SelectGmshFile, "..." );
    m_FemTabLayout.ForceNewLine();

    m_FemTabLayout.AddYGap();

    m_FemTabLayout.SetButtonWidth( 75 );
    m_FemTabLayout.AddButton( m_MassFile, "Mass" );
    m_FemTabLayout.AddOutput( m_MassOutput );
    m_FemTabLayout.SetButtonWidth( m_FemTabLayout.GetRemainX() );
    m_FemTabLayout.AddButton( m_SelectMassFile, "..." );
    m_FemTabLayout.ForceNewLine();

    m_FemTabLayout.AddYGap();

    m_FemTabLayout.SetButtonWidth( 75 );
    m_FemTabLayout.AddButton( m_NastFile, "Nastran" );
    m_FemTabLayout.AddOutput( m_NastOutput );
    m_FemTabLayout.SetButtonWidth( m_FemTabLayout.GetRemainX() );
    m_FemTabLayout.AddButton( m_SelectNastFile, "..." );
    m_FemTabLayout.ForceNewLine();

    m_FemTabLayout.SetButtonWidth( 75 );
    m_FemTabLayout.AddButton( m_NkeyFile, "Nkey" );
    m_FemTabLayout.AddOutput( m_NkeyOutput );
    m_FemTabLayout.SetButtonWidth( m_FemTabLayout.GetRemainX() );
    m_FemTabLayout.AddButton( m_SelectNkeyFile, "..." );
    m_FemTabLayout.ForceNewLine();

    m_FemTabLayout.AddYGap();

    m_FemTabLayout.SetButtonWidth( 75 );
    m_FemTabLayout.AddButton( m_CalcFile, "Calculix" );
    m_FemTabLayout.AddOutput( m_CalcOutput );
    m_FemTabLayout.SetButtonWidth( m_FemTabLayout.GetRemainX() );
    m_FemTabLayout.AddButton( m_SelectCalcFile, "..." );
    m_FemTabLayout.ForceNewLine();

    m_FemTabLayout.AddYGap();

    m_FemTabLayout.SetFitWidthFlag( true );
    m_FemTabLayout.SetSameLineFlag( false );

    m_FemTabLayout.AddDividerBox( "FEM Display" );

    m_FemTabLayout.SetButtonWidth( m_FemTabLayout.GetW() / 2 );

    m_FemTabLayout.SetFitWidthFlag( false );
    m_FemTabLayout.SetSameLineFlag( true );

    m_FemTabLayout.AddButton( m_DrawMeshButton, "Draw Mesh" );
    m_FemTabLayout.AddButton( m_ColorElementsButton, "Color Elements" );
    m_FemTabLayout.ForceNewLine();

    m_FemTabLayout.AddButton( m_DrawNodesToggle, "Draw Nodes" );
    m_FemTabLayout.AddButton( m_DrawElementOrientVecToggle, "Draw Element Orientation" );
    m_FemTabLayout.ForceNewLine();

    m_FemTabLayout.AddButton( m_DrawBCNodesToggle, "Draw BCs" );
    m_FemTabLayout.ForceNewLine();

    m_FemTabLayout.SetFitWidthFlag( true );
    m_FemTabLayout.SetSameLineFlag( false );
    m_FemTabLayout.AddYGap();

    m_FemTabLayout.AddDividerBox( "Element Sets" );

    m_DrawPartSelectBrowser = m_FemTabLayout.AddCheckBrowser( browser_h );
    m_DrawPartSelectBrowser->callback( staticScreenCB, this );

    m_FemTabLayout.AddY( 125 );
    m_FemTabLayout.AddYGap();

    m_FemTabLayout.SetSameLineFlag( true );
    m_FemTabLayout.SetFitWidthFlag( false );

    m_FemTabLayout.SetButtonWidth( m_FemTabLayout.GetW() / 2 );

    m_FemTabLayout.AddButton( m_DrawAllButton, "Draw All Elements" );
    m_FemTabLayout.AddButton( m_HideAllButton, "Hide All Elements" );
    m_FemTabLayout.ForceNewLine();

    //=== CAD TAB ===//
    m_CadTabLayout.SetGroupAndScreen( cadTabGroup, this );


    m_CadTabLayout.SetFitWidthFlag( true );
    m_CadTabLayout.SetSameLineFlag( false );
    m_CadTabLayout.InitWidthHeightVals();

    m_CadTabLayout.AddDividerBox("Surface and Curve Export");

    m_CadTabLayout.AddButton( m_ExportRaw, "Export Raw Points" );

    m_CadTabLayout.InitWidthHeightVals();
    m_CadTabLayout.SetInputWidth( m_CadTabLayout.GetW() - 75 - 55 );
    m_CadTabLayout.SetFitWidthFlag( false );
    m_CadTabLayout.SetSameLineFlag( true );

    m_CadTabLayout.SetButtonWidth( 75 );
    m_CadTabLayout.AddButton(m_CurvFile, ".curv");
    m_CadTabLayout.AddOutput(m_CurvOutput);
    m_CadTabLayout.SetButtonWidth( m_CadTabLayout.GetRemainX() );
    m_CadTabLayout.AddButton(m_SelectCurvFile, "...");
    m_CadTabLayout.ForceNewLine();

    m_CadTabLayout.SetButtonWidth( 75 );
    m_CadTabLayout.AddButton(m_Plot3DFile, ".p3d");
    m_CadTabLayout.AddOutput(m_Plot3DOutput);
    m_CadTabLayout.SetButtonWidth( m_CadTabLayout.GetRemainX() );
    m_CadTabLayout.AddButton(m_SelectPlot3DFile, "...");
    m_CadTabLayout.ForceNewLine();

    m_CadTabLayout.AddYGap();

    m_CadTabLayout.SetButtonWidth( 75 );
    m_CadTabLayout.AddButton( m_SrfFile, ".srf" );
    m_CadTabLayout.AddOutput( m_SrfOutput );
    m_CadTabLayout.SetButtonWidth( m_CadTabLayout.GetRemainX() );
    m_CadTabLayout.AddButton( m_SelectSrfFile, "..." );
    m_CadTabLayout.ForceNewLine();

    m_CadTabLayout.SetSameLineFlag( false );
    m_CadTabLayout.SetFitWidthFlag( true );
    m_CadTabLayout.AddButton( m_XYZIntCurves, "Include X,Y,Z Intersection Curves" );

    m_CadTabLayout.AddYGap();
    m_CadTabLayout.SetFitWidthFlag( true );
    m_CadTabLayout.SetSameLineFlag( false );

    m_CadTabLayout.AddDividerBox( "Trimmed CAD Export" );

    m_CadTabLayout.InitWidthHeightVals();
    m_CadTabLayout.SetFitWidthFlag( false );
    m_CadTabLayout.SetSameLineFlag( true );

    m_CadTabLayout.SetButtonWidth( m_CadTabLayout.GetRemainX() / 4 );

    m_CadTabLayout.AddButton( m_LabelIDToggle, "Geom ID" );
    m_CadTabLayout.AddButton( m_LabelNameToggle, "Geom Name" );
    m_CadTabLayout.AddButton( m_LabelSurfNoToggle, "Surf Number" );
    m_CadTabLayout.AddButton( m_LabelSplitNoToggle, "Split Number" );

    m_CadTabLayout.ForceNewLine();
    m_CadTabLayout.SetSliderWidth( m_CadTabLayout.GetRemainX() / 4 );
    m_CadTabLayout.SetChoiceButtonWidth( m_CadTabLayout.GetRemainX() / 4 );

    m_LabelDelimChoice.AddItem( "Comma" );
    m_LabelDelimChoice.AddItem( "Underscore" );
    m_LabelDelimChoice.AddItem( "Space" );
    m_LabelDelimChoice.AddItem( "None" );
    m_CadTabLayout.AddChoice( m_LabelDelimChoice, "Delimiter" );

    m_LenUnitChoice.AddItem( "MM" );
    m_LenUnitChoice.AddItem( "CM" );
    m_LenUnitChoice.AddItem( "M" );
    m_LenUnitChoice.AddItem( "IN" );
    m_LenUnitChoice.AddItem( "FT" );
    m_LenUnitChoice.AddItem( "YD" );
    m_CadTabLayout.AddChoice( m_LenUnitChoice, "Length Unit" );
    m_CadTabLayout.ForceNewLine();
    m_CadTabLayout.AddYGap();

    m_CadTabLayout.SetInputWidth( m_CadTabLayout.GetW() - 75 - 55 );

    m_CadTabLayout.SetButtonWidth( 75 );
    m_CadTabLayout.AddButton( m_IGESFile, ".igs" );
    m_CadTabLayout.AddOutput( m_IGESOutput );
    m_CadTabLayout.SetButtonWidth( m_CadTabLayout.GetRemainX() );
    m_CadTabLayout.AddButton( m_SelectIGESFile, "..." );

    m_CadTabLayout.ForceNewLine();
    m_CadTabLayout.AddYGap();

    m_CadTabLayout.SetButtonWidth( 75 );
    m_CadTabLayout.AddButton( m_STEPFile, ".stp" );
    m_CadTabLayout.AddOutput( m_STEPOutput );
    m_CadTabLayout.SetButtonWidth( m_CadTabLayout.GetRemainX() );
    m_CadTabLayout.AddButton( m_SelectSTEPFile, "..." );
    m_CadTabLayout.ForceNewLine();

    m_CadTabLayout.SetFitWidthFlag( true );
    m_CadTabLayout.SetSameLineFlag( false );

    m_CadTabLayout.InitWidthHeightVals();
    m_CadTabLayout.SetButtonWidth( 175 );

    //m_CadTabLayout.SetButtonWidth( m_OutputTabLayout.GetRemainX() / 3 );
    //m_CadTabLayout.AddButton( m_STEPMergePointsToggle, "Merge Points" );
    //m_CadTabLayout.SetFitWidthFlag( true );
    m_CadTabLayout.AddSlider( m_STEPTolSlider, "STEP Tolerance", 10, "%5.4g", 0, true );
    //m_CadTabLayout.SetFitWidthFlag( false );
    //m_CadTabLayout.ForceNewLine();

    m_CadTabLayout.SetFitWidthFlag( false );
    m_CadTabLayout.SetSameLineFlag( true );

    m_CadTabLayout.SetButtonWidth( m_CadTabLayout.GetRemainX() / 2 );
    m_CadTabLayout.AddButton( m_STEPShell, "Shell Representation" );
    m_CadTabLayout.AddButton( m_STEPBREP, "BREP Solid Representation" );
    m_CadTabLayout.ForceNewLine();

    m_STEPRepGroup.Init( this );
    m_STEPRepGroup.AddButton( m_STEPShell.GetFlButton() );
    m_STEPRepGroup.AddButton( m_STEPBREP.GetFlButton() );

    m_CadTabLayout.AddYGap();

    m_CadTabLayout.SetFitWidthFlag( true );
    m_CadTabLayout.SetSameLineFlag( false );

    m_CadTabLayout.AddDividerBox( "Intersection Curve Display" );

    m_CadTabLayout.SetFitWidthFlag( false );
    m_CadTabLayout.SetSameLineFlag( true );

    m_CadTabLayout.SetButtonWidth( m_CadTabLayout.GetW() / 2 );

    m_CadTabLayout.AddButton( m_DrawIsect, "Show Intersection Curves");
    m_CadTabLayout.AddButton( m_DrawBorder, "Show Border Curves");
    m_CadTabLayout.ForceNewLine();

    m_CadTabLayout.AddButton( m_ShowCurve, "Show Curves");
    m_CadTabLayout.AddButton( m_ShowPts, "Show Points");
    m_CadTabLayout.ForceNewLine();

    m_CadTabLayout.AddButton( m_ShowRaw, "Show Raw Curve");
    m_CadTabLayout.AddButton( m_ShowBinAdapt, "Show Binary Adapted");
    m_CadTabLayout.ForceNewLine();

    // Set initial values
    m_FeaCurrMainSurfIndx = 0;
    m_SelectedFeaPartChoice = 0;
    m_CurrDispGroup = NULL;
    m_SelectedBCPartChoice = 0;
    m_SelectedBCSubSurfChoice = 0;
}

StructScreen::~StructScreen()
{
    m_ConsoleDisplay->buffer( NULL );
    delete m_ConsoleBuffer;
}

void StructScreen::LoadGeomChoice()
{
    //==== Geom Choice ====//
    m_GeomChoice.ClearItems();
    m_GeomIDVec.clear();

    Vehicle* veh = m_ScreenMgr->GetVehiclePtr();
    if ( veh )
    {
        vector< Geom* > geom_vec = veh->FindGeomVec( veh->GetGeomVec() );

        // TODO:  This looks like it would be much easier to use a GeomPicker and the AddExcludeType() method.
        map <string, int> CompIDMap;
        int icomp = 0;
        for ( int i = 0; i < (int)geom_vec.size(); ++i )
        {
            string disp_geom_name = std::to_string( icomp + 1 ) + ". " + geom_vec[i]->GetName();
            int geom_type = geom_vec[i]->GetType().m_Type;

            if ( geom_type != BLANK_GEOM_TYPE &&
                 geom_type != PT_CLOUD_GEOM_TYPE &&
                 geom_type != HINGE_GEOM_TYPE &&
                 geom_type != MESH_GEOM_TYPE &&
                 geom_type != HUMAN_GEOM_TYPE )
            {
                m_GeomChoice.AddItem( disp_geom_name );
                CompIDMap[geom_vec[i]->GetID()] = icomp;
                m_GeomIDVec.push_back( geom_vec[i]->GetID() );
                icomp++;
            }
        }
        m_GeomChoice.UpdateItems();

        Geom* currgeom = veh->FindGeom( m_SelectedGeomID );

        if ( m_SelectedGeomID.length() == 0 && m_GeomIDVec.size() > 0 )
        {
            // Handle default case
            m_SelectedGeomID = m_GeomIDVec[0];
        }
        else if ( !currgeom && m_GeomIDVec.size() > 0 )
        {
            m_SelectedGeomID = m_GeomIDVec[0];
        }
        
        m_GeomChoice.SetVal( CompIDMap[m_SelectedGeomID] );
    }
}

void StructScreen::UpdateStructBrowser()
{
    //==== Structure Browser ====//
    int scroll_pos = m_StructureSelectBrowser->position();
    int h_pos = m_StructureSelectBrowser->hposition();
    m_StructureSelectBrowser->clear();
    m_CurrFeaMeshChoice.ClearItems();
    m_StructIDs.clear();

    Vehicle* veh = m_ScreenMgr->GetVehiclePtr();
    if ( !veh )
    {
        return;
    }

    m_StructureSelectBrowser->column_char( ':' );

    char str[256];

    m_StructureSelectBrowser->add( "@b@.NAME:@b@.GEOM:@b@.SURFACE" );

    string struct_name, parent_geom_name;
    int struct_surf_ind;

    // Populate browser with added structures
    vector< FeaStructure* > structVec = StructureMgr.GetAllFeaStructs();
    if ( structVec.size() > 0 )
    {
        for ( int i = 0; i < (int)structVec.size(); i++ )
        {
            struct_name = structVec[i]->GetName();
            struct_surf_ind = structVec[i]->GetFeaStructMainSurfIndx();

            Geom* parent = veh->FindGeom( structVec[i]->GetParentGeomID() );
            if ( parent )
            {
                parent_geom_name = parent->GetName();
            }

            snprintf( str, sizeof( str ),  "%s:%s:Surf_%d", struct_name.c_str(), parent_geom_name.c_str(), struct_surf_ind );
            m_StructureSelectBrowser->add( str );
            m_CurrFeaMeshChoice.AddItem( struct_name, i );

            m_StructIDs.push_back( structVec[i]->GetID() );
        }
        m_CurrFeaMeshChoice.UpdateItems();

        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            m_StructureSelectBrowser->select( StructureMgr.m_CurrStructIndex() + 2 );
            m_CurrFeaMeshChoice.Update( StructureMgr.m_CurrStructIndex.GetID() );
        }
    }

    // Make sure FeaMeshMgr is consistent.
    if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
    {
        FeaMeshMgr.SetFeaMeshStructID( m_StructIDs[ StructureMgr.m_CurrStructIndex() ] );
    }

    m_StructureSelectBrowser->position( scroll_pos );
    m_StructureSelectBrowser->hposition( h_pos );
}

void StructScreen::UpdateFeaPartBrowser()
{
    //==== FeaPart Browser ====//
    int scroll_pos = m_FeaPartSelectBrowser->position();
    int h_pos = m_FeaPartSelectBrowser->hposition();
    m_FeaPartSelectBrowser->clear();

    m_FeaPartSelectBrowser->column_char( ':' );

    char str[256];

    m_FeaPartSelectBrowser->add( "@b@.NAME:@b@.TYPE:@b@.SHELL:@b@.PROPERTY:@b@.CAP:@b@.PROPERTY" );

    string fea_name, fea_type, shell, shell_prop, cap, cap_prop;

    if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
    {
        vector< FeaStructure* > structVec = StructureMgr.GetAllFeaStructs();
        vector<FeaPart*> feaprt_vec = structVec[StructureMgr.m_CurrStructIndex()]->GetFeaPartVec();

        for ( int i = 0; i < (int)feaprt_vec.size(); i++ )
        {
            fea_name = feaprt_vec[i]->GetName();
            fea_type = FeaPart::GetTypeName( feaprt_vec[i]->GetType() );

            if ( feaprt_vec[i]->m_IncludedElements() == vsp::FEA_SHELL || feaprt_vec[i]->m_IncludedElements() == vsp::FEA_SHELL_AND_BEAM )
            {
                shell = "     X";

                FeaProperty* prop = StructureMgr.GetFeaProperty( feaprt_vec[i]->m_FeaPropertyIndex() );
                if ( prop )
                {
                    shell_prop = prop->GetName();
                }
            }
            else
            {
                shell = "";
                shell_prop = "N/A";
            }

            if ( feaprt_vec[i]->m_IncludedElements() == vsp::FEA_BEAM || feaprt_vec[i]->m_IncludedElements() == vsp::FEA_SHELL_AND_BEAM )
            {
                cap = "   X";

                FeaProperty* prop = StructureMgr.GetFeaProperty( feaprt_vec[i]->m_CapFeaPropertyIndex() );
                if ( prop )
                {
                    cap_prop = prop->GetName();
                }
            }
            else
            {
                cap = "";
                cap_prop = "N/A";
            }

            if ( feaprt_vec[i]->GetType() == vsp::FEA_FIX_POINT )
            {
                shell = "";
                shell_prop = "N/A";
                cap = "";
                cap_prop = "N/A";
            }
            else if ( feaprt_vec[i]->GetType() == vsp::FEA_SKIN )
            {
                FeaSkin* skin = dynamic_cast<FeaSkin*>( feaprt_vec[i] );
                assert( skin );

                if ( skin->m_RemoveSkinFlag() )
                {
                    shell = "";
                    shell_prop = "N/A";
                }
            }

            snprintf( str, sizeof( str ),  "%s:%s:%s:%s:%s:%s:", fea_name.c_str(), fea_type.c_str(), shell.c_str(), shell_prop.c_str(), cap.c_str(), cap_prop.c_str() );
            m_FeaPartSelectBrowser->add( str );
        }

        vector<SubSurface*> subsurf_vec = structVec[StructureMgr.m_CurrStructIndex()]->GetFeaSubSurfVec();

        for ( int i = 0; i < (int)subsurf_vec.size(); i++ )
        {
            fea_name = subsurf_vec[i]->GetName();
            fea_type = SubSurface::GetTypeName( subsurf_vec[i]->GetType() );

            if ( subsurf_vec[i]->m_KeepDelShellElements() == vsp::FEA_KEEP )
            {
                shell = "     X";

                FeaProperty* prop = StructureMgr.GetFeaProperty( subsurf_vec[i]->m_FeaPropertyIndex() );
                if ( prop )
                {
                    shell_prop = prop->GetName();
                }
            }
            else
            {
                shell = "";
                shell_prop = "N/A";
            }

            if ( subsurf_vec[i]->m_CreateBeamElements() )
            {
                cap = "   X";

                FeaProperty* prop = StructureMgr.GetFeaProperty( subsurf_vec[i]->m_CapFeaPropertyIndex() );
                if ( prop )
                {
                    cap_prop = prop->GetName();
                }
            }
            else
            {
                cap = "";
                cap_prop = "N/A";
            }

            snprintf( str, sizeof( str ),  "%s:%s:%s:%s:%s:%s:", fea_name.c_str(), fea_type.c_str(), shell.c_str(), shell_prop.c_str(), cap.c_str(), cap_prop.c_str() );
            m_FeaPartSelectBrowser->add( str );
        }

        for ( size_t i = 0; i < m_SelectedPartIndexVec.size(); i++ )
        {
            if ( structVec[StructureMgr.m_CurrStructIndex()]->ValidFeaPartInd( m_SelectedPartIndexVec[i] ) ||
                 structVec[StructureMgr.m_CurrStructIndex()]->ValidFeaSubSurfInd( m_SelectedPartIndexVec[i] - structVec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() ) )
            {
                m_FeaPartSelectBrowser->select( m_SelectedPartIndexVec[i] + 2 );
            }
        }
    }

    m_FeaPartSelectBrowser->position( scroll_pos );
    m_FeaPartSelectBrowser->hposition( h_pos );
}

void StructScreen::UpdateDrawPartBrowser()
{
    if ( !FeaMeshMgr.GetMeshPtr() )
    {
        return;
    }

    //==== Draw Part Browser ====//
    int scroll_pos = m_DrawPartSelectBrowser->position();
    m_DrawPartSelectBrowser->clear();

    vector < int > draw_browser_index_vec = FeaMeshMgr.GetMeshPtr()->GetDrawBrowserIndexVec();
    vector < string > draw_browser_name_vec = FeaMeshMgr.GetMeshPtr()->GetDrawBrowserNameVec();
    vector < bool > draw_element_flag_vec = FeaMeshMgr.GetMeshPtr()->GetDrawElementFlagVec();
    vector < bool > draw_cap_flag_vec = FeaMeshMgr.GetMeshPtr()->GetDrawCapFlagVec();

    for ( unsigned int i = 0; i < draw_browser_name_vec.size(); i++ )
    {
        if ( draw_browser_name_vec[i].find( "CAP" ) != std::string::npos )
        {
            m_DrawPartSelectBrowser->add( draw_browser_name_vec[i].c_str(), draw_cap_flag_vec[draw_browser_index_vec[i]] );
        }
        else
        {
            m_DrawPartSelectBrowser->add( draw_browser_name_vec[i].c_str(), draw_element_flag_vec[draw_browser_index_vec[i]] );
        }
    }

    m_DrawPartSelectBrowser->position( scroll_pos );
}

void StructScreen::UpdateMainSurfChoice()
{
    //==== Main Surf Choice ====//
    m_SurfSel.ClearItems();

    Vehicle*  veh = m_ScreenMgr->GetVehiclePtr();

    if ( veh )
    {
        Geom* currgeom = veh->FindGeom( m_SelectedGeomID );

        if ( currgeom )
        {
            int nmain = currgeom->GetNumMainSurfs();
            for ( int i = 0; i < nmain; ++i )
            {
                m_SurfSel.AddItem( string( "Surf_" + std::to_string( i ) ) );
            }
            m_SurfSel.UpdateItems();

            if ( m_FeaCurrMainSurfIndx < 0 || m_FeaCurrMainSurfIndx >= nmain )
            {
                m_FeaCurrMainSurfIndx = 0;
            }
            m_SurfSel.SetVal( m_FeaCurrMainSurfIndx );
        }
    }
}

void StructScreen::UpdateFeaPartChoice()
{
    //==== FeaPart Type Choice ====//
    m_FeaPartChoice.ClearItems();

    Vehicle*  veh = m_ScreenMgr->GetVehiclePtr();

    if ( veh )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            FeaStructure* curr_struct = StructureMgr.GetAllFeaStructs()[StructureMgr.m_CurrStructIndex()];

            if ( curr_struct )
            {
                Geom* currgeom = veh->FindGeom( curr_struct->GetParentGeomID() );

                if ( currgeom )
                {
                    m_FeaPartChoice.AddItem( FeaPart::GetTypeName( vsp::FEA_SLICE ), vsp::FEA_SLICE );
                    m_FeaPartChoice.AddItem( FeaPart::GetTypeName( vsp::FEA_RIB ), vsp::FEA_RIB );
                    m_FeaPartChoice.AddItem( FeaPart::GetTypeName( vsp::FEA_SPAR ), vsp::FEA_SPAR );
                    m_FeaPartChoice.AddItem( FeaPart::GetTypeName( vsp::FEA_FIX_POINT ), vsp::FEA_FIX_POINT );
                    m_FeaPartChoice.AddItem( FeaPart::GetTypeName( vsp::FEA_DOME ), vsp::FEA_DOME );
                    m_FeaPartChoice.AddItem( FeaPart::GetTypeName( vsp::FEA_RIB_ARRAY ), vsp::FEA_RIB_ARRAY );
                    m_FeaPartChoice.AddItem( FeaPart::GetTypeName( vsp::FEA_SLICE_ARRAY ), vsp::FEA_SLICE_ARRAY );
                    m_FeaPartChoice.AddItem( FeaPart::GetTypeName( vsp::FEA_TRIM ), vsp::FEA_TRIM );

                    // Number of non-subsurface types.  Used as an offset when indexing m_FeaPartChoice, but needing
                    // to index into subsurface part types.
                    m_FeaPartChoiceSubSurfOffset = vsp::FEA_NUM_TYPES;

                    m_FeaPartChoice.AddItem( SubSurface::GetTypeName( vsp::SS_LINE ), vsp::SS_LINE + m_FeaPartChoiceSubSurfOffset );
                    m_FeaPartChoice.AddItem( SubSurface::GetTypeName( vsp::SS_RECTANGLE ), vsp::SS_RECTANGLE + m_FeaPartChoiceSubSurfOffset  );
                    m_FeaPartChoice.AddItem( SubSurface::GetTypeName( vsp::SS_ELLIPSE ), vsp::SS_ELLIPSE + m_FeaPartChoiceSubSurfOffset  );
                    m_FeaPartChoice.AddItem( SubSurface::GetTypeName( vsp::SS_CONTROL ), vsp::SS_CONTROL + m_FeaPartChoiceSubSurfOffset  );
                    m_FeaPartChoice.AddItem( SubSurface::GetTypeName( vsp::SS_LINE_ARRAY ), vsp::SS_LINE_ARRAY + m_FeaPartChoiceSubSurfOffset  );
                    m_FeaPartChoice.AddItem( SubSurface::GetTypeName( vsp::SS_FINITE_LINE ), vsp::SS_FINITE_LINE + m_FeaPartChoiceSubSurfOffset  );

                    if ( currgeom->GetType().m_Type == MS_WING_GEOM_TYPE )
                    {
                        m_FeaPartChoice.SetFlagByVal( vsp::FEA_RIB, 0 ); // FEA_RIB
                        m_FeaPartChoice.SetFlagByVal( vsp::FEA_SPAR, 0 ); // FEA_SPAR
                        m_FeaPartChoice.SetFlagByVal( vsp::FEA_RIB_ARRAY, 0 ); // FEA_RIB_ARRAY
                        m_FeaPartChoice.SetFlagByVal( vsp::SS_CONTROL + m_FeaPartChoiceSubSurfOffset, 0 ); // SS_CONTROL
                    }
                    else
                    {
                        m_FeaPartChoice.SetFlagByVal( vsp::FEA_RIB, FL_MENU_INACTIVE );
                        m_FeaPartChoice.SetFlagByVal( vsp::FEA_SPAR, FL_MENU_INACTIVE );
                        m_FeaPartChoice.SetFlagByVal( vsp::FEA_RIB_ARRAY, FL_MENU_INACTIVE );
                        m_FeaPartChoice.SetFlagByVal( vsp::SS_CONTROL + m_FeaPartChoiceSubSurfOffset, FL_MENU_INACTIVE );
                    }

                    if ( currgeom->GetType().m_Type == FUSELAGE_GEOM_TYPE || currgeom->GetType().m_Type == POD_GEOM_TYPE || currgeom->GetType().m_Type == STACK_GEOM_TYPE ) //TODO: Improve
                    {
                        m_FeaPartChoice.SetFlagByVal( vsp::FEA_DOME, 0 ); // FEA_DOME
                    }
                    else
                    {
                        m_FeaPartChoice.SetFlagByVal( vsp::FEA_DOME, FL_MENU_INACTIVE );
                    }



                    m_FeaPartChoice.UpdateItems();

                    m_FeaPartChoice.SetVal( m_SelectedFeaPartChoice );
                }
            }
        }
    }
}

void StructScreen::UpdateFeaPropertyBrowser()
{
    //==== FeaPart Browser ====//
    int scroll_pos = m_FeaPropertySelectBrowser->position();
    int h_pos = m_FeaPropertySelectBrowser->hposition();
    m_FeaPropertySelectBrowser->clear();

    m_FeaPropertySelectBrowser->column_char( ':' );

    char str[256];

    m_FeaPropertySelectBrowser->add( "@b@.NAME:@b@.TYPE:@b@.XSEC:@b@.MATERIAL:" );

    string prop_name, prop_type, prop_xsec, prop_mat;

    vector< FeaProperty* > property_vec = StructureMgr.GetFeaPropertyVec();

    for ( int i = 0; i < (int)property_vec.size(); i++ )
    {
        prop_name = property_vec[i]->GetName();
        prop_type = property_vec[i]->GetTypeName();

        if ( property_vec[i]->m_FeaPropertyType() == vsp::FEA_SHELL )
        {
            prop_xsec = "N/A";
        }
        else
        {
            prop_xsec = property_vec[i]->GetXSecName();
        }

        FeaMaterial* fea_mat = StructureMgr.GetFeaMaterial( property_vec[i]->m_FeaMaterialIndex() );
        if ( fea_mat )
        {
            prop_mat = fea_mat->GetName();
        }

        snprintf( str, sizeof( str ),  "%s:%s:%s:%s:", prop_name.c_str(), prop_type.c_str(), prop_xsec.c_str(), prop_mat.c_str() );

        m_FeaPropertySelectBrowser->add( str );
    }

    if ( StructureMgr.ValidFeaPropertyInd( StructureMgr.GetCurrPropertyIndex() ) )
    {
        m_FeaPropertySelectBrowser->select( StructureMgr.GetCurrPropertyIndex() + 2 );
    }

    m_FeaPropertySelectBrowser->position( scroll_pos );
    m_FeaPropertySelectBrowser->hposition( h_pos );
}

void StructScreen::UpdateFeaMaterialBrowser()
{
    //==== FeaMaterial Browser ====//
    int scroll_pos = m_FeaMaterialSelectBrowser->position();
    m_FeaMaterialSelectBrowser->clear();

    vector< FeaMaterial* > material_vec = StructureMgr.GetFeaMaterialVec();

    for ( int i = 0; i < (int)material_vec.size(); i++ )
    {
        string mat_name = material_vec[i]->GetName();
        fltk_unicode_subscripts( mat_name );
        fltk_unicode_plusminus( mat_name );
        m_FeaMaterialSelectBrowser->add( mat_name.c_str() );
    }

    if ( StructureMgr.ValidFeaMaterialInd( StructureMgr.GetCurrMaterialIndex() ) )
    {
        m_MaterialEditSubGroup.Show();
        m_FeaMaterialSelectBrowser->select( StructureMgr.GetCurrMaterialIndex() + 1 );
    }
    else
    {
        m_MaterialEditSubGroup.Hide();
    }

    m_FeaMaterialSelectBrowser->position( scroll_pos );
}

void StructScreen::UpdateFeaMaterialChoice()
{
    //==== Material Choice ====//
    m_FeaShellMaterialChoice.ClearItems();
    m_FeaBeamMaterialChoice.ClearItems();

    Vehicle*  veh = m_ScreenMgr->GetVehiclePtr();

    if ( veh )
    {
        vector< FeaMaterial* > material_vec = StructureMgr.GetFeaMaterialVec();

        for ( int i = 0; i < material_vec.size(); ++i )
        {
            string mat_name = material_vec[i]->GetName();
            fltk_unicode_subscripts( mat_name );
            fltk_unicode_plusminus( mat_name );

            m_FeaShellMaterialChoice.AddItem( mat_name, i );

            if ( material_vec[i]->m_FeaMaterialType() == vsp::FEA_ISOTROPIC )
            {
                m_FeaBeamMaterialChoice.AddItem( mat_name, i );
            }
        }
        m_FeaShellMaterialChoice.UpdateItems();
        m_FeaBeamMaterialChoice.UpdateItems();

        if ( StructureMgr.ValidFeaPropertyInd( StructureMgr.GetCurrPropertyIndex() ) )
        {
            FeaProperty* fea_prop = StructureMgr.GetFeaPropertyVec()[StructureMgr.GetCurrPropertyIndex()];

            if ( fea_prop )
            {
                // Update all FeaPart Material Choices ( Only Selected Property Visible )
                m_FeaShellMaterialChoice.SetVal( fea_prop->m_FeaMaterialIndex() );
                m_FeaBeamMaterialChoice.SetVal( fea_prop->m_FeaMaterialIndex() );
            }
        }
    }
}

void StructScreen::UpdateFeaBCBrowser()
{
    //==== FeaPart Browser ====//
    int scroll_pos = m_FeaBCSelectBrowser->position();
    int h_pos = m_FeaBCSelectBrowser->hposition();
    m_FeaBCSelectBrowser->clear();

    m_FeaBCSelectBrowser->column_char( ':' );

    if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
    {
        FeaStructure *curr_struct = StructureMgr.GetAllFeaStructs()[ StructureMgr.m_CurrStructIndex() ];

        if ( curr_struct )
        {
            vector< FeaBC* > bc_vec = curr_struct->GetFeaBCVec();

            for ( int i = 0; i < (int)bc_vec.size(); i++ )
            {
                string label = bc_vec[i]->GetDescription();

                m_FeaBCSelectBrowser->add( label.c_str() );
            }

            if ( curr_struct->ValidFeaBCInd( StructureMgr.GetCurrBCIndex() ) )
            {
                m_FeaBCCommonGroup.Show();
                m_FeaBCSelectBrowser->select( StructureMgr.GetCurrBCIndex() + 1 );
            }
            else
            {
                m_FeaBCCommonGroup.Hide();
            }

            m_FeaBCSelectBrowser->position( scroll_pos );
            m_FeaBCSelectBrowser->hposition( h_pos );
        }
    }
}

void StructScreen::UpdateBCPartChoice()
{
    //==== FixPoint Parent Surf Choice ====//
    m_FeaBCPartChoice.ClearItems();
    m_FeaBCPartChoiceIDVec.clear();

    int istruct = StructureMgr.m_CurrStructIndex();

    if ( StructureMgr.ValidTotalFeaStructInd( istruct ) )
    {
        vector < FeaStructure * > structVec = StructureMgr.GetAllFeaStructs();
        FeaStructure * fea_struct = structVec[ istruct ];
        vector < FeaPart * > feaprt_vec = fea_struct->GetFeaPartVec(); // Does not include subsurfaces

        // Loop over all parts in current structure
        for ( size_t i = 0; i < feaprt_vec.size(); i++ )
        {
            m_FeaBCPartChoice.AddItem( string( feaprt_vec[ i ]->GetName() ) );
            m_FeaBCPartChoiceIDVec.push_back( feaprt_vec[ i ]->GetID() );
        }

        m_FeaBCPartChoice.UpdateItems();
    }
}

void StructScreen::UpdateBCSubSurfChoice()
{
    //==== FixPoint Parent Surf Choice ====//
    m_FeaBCSubSurfChoice.ClearItems();
    m_FeaBCSubSurfChoiceIDVec.clear();

    int istruct = StructureMgr.m_CurrStructIndex();

    if ( StructureMgr.ValidTotalFeaStructInd( istruct ) )
    {
        vector < FeaStructure * > structVec = StructureMgr.GetAllFeaStructs();
        FeaStructure * fea_struct = structVec[ istruct ];
        vector < SubSurface * > subsurf_vec = fea_struct->GetFeaSubSurfVec(); // Does not include subsurfaces

        // Loop over all parts in current structure
        for ( size_t i = 0; i < subsurf_vec.size(); i++ )
        {
            m_FeaBCSubSurfChoice.AddItem( string( subsurf_vec[ i ]->GetName() ) );
            m_FeaBCSubSurfChoiceIDVec.push_back( subsurf_vec[ i ]->GetID() );
        }

        m_FeaBCSubSurfChoice.UpdateItems();
    }
}

void StructScreen::BeamXSecDispGroup( GroupLayout* group )
{
    if ( m_CurBeamXSecDispGroup == group && group )
    {
        return;
    }

    m_GenXSecGroup.Hide();
    m_CircXSecGroup.Hide();
    m_PipeXSecGroup.Hide();
    m_IXSecGroup.Hide();
    m_RectXSecGroup.Hide();
    m_BoxXSecGroup.Hide();

    m_CurBeamXSecDispGroup = group;

    if ( group )
    {
        group->Show();
    }
}


void StructScreen::CloseCallBack( Fl_Widget *w )
{
    Hide();

    m_ScreenMgr->SetUpdateFlag( true );
}

void StructScreen::Show()
{
    m_FLTK_Window->show();

    m_ScreenMgr->SetUpdateFlag( true );
}

void StructScreen::FeaPropertyDispGroup( GroupLayout* group )
{
    if ( m_CurFeaPropDispGroup == group && group )
    {
        return;
    }

    m_FeaPropertyShellGroup.Hide();
    m_FeaPropertyBeamGroup.Hide();
    m_FeaPropertyCommonGroup.Hide();

    m_CurFeaPropDispGroup = group;

    if ( group )
    {
        group->Show();
        m_FeaPropertyCommonGroup.Show(); // Always show the Common Group if any other subsurface group is being displayed.
    }
}

void StructScreen::FeaStructDispGroup( GroupLayout* group )
{
    if ( m_CurrDispGroup == group && group )
    {
        return;
    }

    m_StructGroup.Hide();
    m_PartGroup.Hide();
    m_StructGeneralGroup.Hide();
    m_StructWingGroup.Hide();

    if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
    {
        m_StructGroup.Show();

        FeaStructure* curr_struct = StructureMgr.GetAllFeaStructs()[StructureMgr.m_CurrStructIndex()];

        Vehicle*  veh = m_ScreenMgr->GetVehiclePtr();

        if ( veh && curr_struct )
        {
            Geom* currgeom = veh->FindGeom( curr_struct->GetParentGeomID() );

            if ( currgeom )
            {
                if ( currgeom->GetType().m_Type == MS_WING_GEOM_TYPE )
                {
                    m_StructWingGroup.Show();
                    m_StructGeneralGroup.Hide();
                }
                else
                {
                    m_StructGeneralGroup.Show();
                    m_StructWingGroup.Hide();
                }
            }
        }
    }

    m_CurrDispGroup = group;

    if ( group )
    {
        group->Show();
    }
}

void StructScreen::UpdateUnitLabels()
{
    Vehicle* veh = m_ScreenMgr->GetVehiclePtr();

    if ( veh )
    {
        string squared( 1, (char) 178 );
        string cubed( 1, (char) 179 );
        string deg( 1, (char) 176 );
        string thick_unit, area_unit, area_moment_inertia_unit, young_mod_unit, density_unit, temp_unit;

        switch ( veh->m_StructUnit() )
        {
        case vsp::SI_UNIT:
        density_unit = "kg/m" + cubed;
        thick_unit = "m";
        area_unit = "m" + squared;
        area_moment_inertia_unit = "m^4";
        young_mod_unit = "Pa";
        temp_unit = "1/K";
        break;

        case vsp::CGS_UNIT:
        density_unit = "g/cm" + cubed;
        thick_unit = "cm";
        area_unit = "cm" + squared;
        area_moment_inertia_unit = "cm^4";
        young_mod_unit = "Ba";
        temp_unit = "1/K";
        break;

        case vsp::MPA_UNIT:
        density_unit = "t/mm" + cubed; // or Mg/
        thick_unit = "mm";
        area_unit = "mm" + squared;
        area_moment_inertia_unit = "mm^4";
        young_mod_unit = "MPa";
        temp_unit = "1/K";
        break;

        case vsp::BFT_UNIT:
        density_unit = "slug/ft" + cubed;
        thick_unit = "ft";
        area_unit = "ft" + squared;
        area_moment_inertia_unit = "ft^4";
        young_mod_unit = "lbf/ft" + squared;
        temp_unit = "1/" + deg + "R";
        break;

        case vsp::BIN_UNIT:
        density_unit = "lbf*sec" + squared + "/in^4";
        thick_unit = "in";
        area_unit = "in" + squared;
        area_moment_inertia_unit = "in^4";
        young_mod_unit = "psi";
        temp_unit = "1/" + deg + "R";
        break;
        }

        m_MatDensityUnit.GetFlButton()->copy_label( density_unit.c_str() );
        m_MatElasticModUnit.GetFlButton()->copy_label( young_mod_unit.c_str() );
        m_MatThermalExCoeffUnit.GetFlButton()->copy_label( temp_unit.c_str() );
        m_MatShearModUnit.GetFlButton()->copy_label( young_mod_unit.c_str() );

        m_PropThickUnit.GetFlButton()->copy_label( thick_unit.c_str() );

        m_PropAreaUnit.GetFlButton()->copy_label( area_unit.c_str() );
        m_PropIzzUnit.GetFlButton()->copy_label( area_moment_inertia_unit.c_str() );
        m_PropIyyUnit.GetFlButton()->copy_label( area_moment_inertia_unit.c_str() );
        m_PropIzyUnit.GetFlButton()->copy_label( area_moment_inertia_unit.c_str() );
        m_PropIxxUnit.GetFlButton()->copy_label( area_moment_inertia_unit.c_str() );

        m_CircDim1Unit.GetFlButton()->copy_label( thick_unit.c_str() );

        m_PipeDim1Unit.GetFlButton()->copy_label( thick_unit.c_str() );
        m_PipeDim2Unit.GetFlButton()->copy_label( thick_unit.c_str() );

        m_IDim1Unit.GetFlButton()->copy_label( thick_unit.c_str() );
        m_IDim2Unit.GetFlButton()->copy_label( thick_unit.c_str() );
        m_IDim3Unit.GetFlButton()->copy_label( thick_unit.c_str() );
        m_IDim4Unit.GetFlButton()->copy_label( thick_unit.c_str() );
        m_IDim5Unit.GetFlButton()->copy_label( thick_unit.c_str() );
        m_IDim6Unit.GetFlButton()->copy_label( thick_unit.c_str() );

        m_RectDim1Unit.GetFlButton()->copy_label( thick_unit.c_str() );
        m_RectDim2Unit.GetFlButton()->copy_label( thick_unit.c_str() );

        m_BoxDim1Unit.GetFlButton()->copy_label( thick_unit.c_str() );
        m_BoxDim2Unit.GetFlButton()->copy_label( thick_unit.c_str() );
        m_BoxDim3Unit.GetFlButton()->copy_label( thick_unit.c_str() );
        m_BoxDim4Unit.GetFlButton()->copy_label( thick_unit.c_str() );
    }
}

bool StructScreen::Update()
{
    Vehicle* veh = m_ScreenMgr->GetVehiclePtr();

    if ( veh )
    {
        FeaStructDispGroup( NULL ); // Hide all initially

        m_StructUnitChoice.Update( veh->m_StructUnit.GetID() );

        //==== Update Unit Labels ====//
        UpdateUnitLabels();

        //===== Geom Choice Update =====//
        LoadGeomChoice();

        //==== Main Surf Choice Update ====//
        UpdateMainSurfChoice();

        //===== Structure Browser Update =====//
        UpdateStructBrowser();

        //===== Draw Part Browser Update =====//
        UpdateDrawPartBrowser();

        //===== FeaPart Browser Update =====//
        UpdateFeaPartBrowser();

        //==== Update FeaPart Choice ====//
        UpdateFeaPartChoice();

        //===== FeaProperty Update =====//
        UpdateFeaPropertyBrowser();

        if ( StructureMgr.ValidFeaPropertyInd( StructureMgr.GetCurrPropertyIndex() ) )
        {
            FeaProperty* fea_prop = StructureMgr.GetFeaPropertyVec()[StructureMgr.GetCurrPropertyIndex()];
            if ( fea_prop )
            {
                m_FeaPropertyNameInput.Update( fea_prop->GetName() );

                if ( fea_prop->m_FeaPropertyType() == vsp::FEA_SHELL )
                {
                    m_PropThickSlider.Update( fea_prop->m_Thickness.GetID() );

                    FeaPropertyDispGroup( &m_FeaPropertyShellGroup );
                }
                else if ( fea_prop->m_FeaPropertyType() == vsp::FEA_BEAM )
                {
                    m_FeaBeamXSecChoice.Update( fea_prop->m_CrossSectType.GetID() );

                    FeaPropertyDispGroup( &m_FeaPropertyBeamGroup );

                    m_ShowFeaBeamXSecButton.Activate();

                    if ( fea_prop->m_CrossSectType() == vsp::FEA_XSEC_GENERAL )
                    {
                        m_ShowFeaBeamXSecButton.Deactivate();

                        m_PropAreaSlider.Update( fea_prop->m_CrossSecArea.GetID() );
                        m_PropIzzSlider.Update( fea_prop->m_Izz.GetID() );
                        m_PropIyySlider.Update( fea_prop->m_Iyy.GetID() );
                        m_PropIzySlider.Update( fea_prop->m_Izy.GetID() );
                        m_PropIxxSlider.Update( fea_prop->m_Ixx.GetID() );

                        BeamXSecDispGroup( &m_GenXSecGroup );
                    }
                    else if ( fea_prop->m_CrossSectType() == vsp::FEA_XSEC_CIRC )
                    {
                        m_CircDim1Slider.Update( fea_prop->m_Dim1.GetID() );

                        BeamXSecDispGroup( &m_CircXSecGroup );
                    }
                    else if ( fea_prop->m_CrossSectType() == vsp::FEA_XSEC_PIPE )
                    {
                        m_PipeDim1Slider.Update( fea_prop->m_Dim1.GetID() );
                        m_PipeDim2Slider.Update( fea_prop->m_Dim2.GetID() );

                        BeamXSecDispGroup( &m_PipeXSecGroup );
                    }
                    else if ( fea_prop->m_CrossSectType() == vsp::FEA_XSEC_I )
                    {
                        m_IDim1Slider.Update( fea_prop->m_Dim1.GetID() );
                        m_IDim2Slider.Update( fea_prop->m_Dim2.GetID() );
                        m_IDim3Slider.Update( fea_prop->m_Dim3.GetID() );
                        m_IDim4Slider.Update( fea_prop->m_Dim4.GetID() );
                        m_IDim5Slider.Update( fea_prop->m_Dim5.GetID() );
                        m_IDim6Slider.Update( fea_prop->m_Dim6.GetID() );

                        BeamXSecDispGroup( &m_IXSecGroup );
                    }
                    else if ( fea_prop->m_CrossSectType() == vsp::FEA_XSEC_RECT )
                    {
                        m_RectDim1Slider.Update( fea_prop->m_Dim1.GetID() );
                        m_RectDim2Slider.Update( fea_prop->m_Dim2.GetID() );

                        BeamXSecDispGroup( &m_RectXSecGroup );
                    }
                    else if ( fea_prop->m_CrossSectType() == vsp::FEA_XSEC_BOX )
                    {
                        m_BoxDim1Slider.Update( fea_prop->m_Dim1.GetID() );
                        m_BoxDim2Slider.Update( fea_prop->m_Dim2.GetID() );
                        m_BoxDim3Slider.Update( fea_prop->m_Dim3.GetID() );
                        m_BoxDim4Slider.Update( fea_prop->m_Dim4.GetID() );

                        BeamXSecDispGroup( &m_BoxXSecGroup );
                    }
                }
            }
            else
            {
                FeaPropertyDispGroup( NULL );
            }
        }
        else
        {
            FeaPropertyDispGroup( NULL );
        }

        //===== FeaMaterial Update =====//
        UpdateFeaMaterialBrowser();
        UpdateFeaMaterialChoice();

        if ( StructureMgr.ValidFeaMaterialInd( StructureMgr.GetCurrMaterialIndex() ) )
        {
            FeaMaterial* fea_mat = StructureMgr.GetFeaMaterialVec()[StructureMgr.GetCurrMaterialIndex()];
            if ( fea_mat )
            {
                m_FeaMaterialTypeChoice.Update( fea_mat->m_FeaMaterialType.GetID() );
                m_FeaMaterialNameInput.Update( fea_mat->GetName() );
                m_MatDensitySlider.Update( fea_mat->m_MassDensity.GetID() );
                m_MatElasticModSlider.Update( fea_mat->m_ElasticModulus.GetID() );
                m_MatPoissonSlider.Update( fea_mat->m_PoissonRatio.GetID() );

                char str[256];
                snprintf( str, sizeof( str ),  "%5.3g", fea_mat->GetShearModulus() );
                m_MatShearModOutput.Update( str );

                m_MatThermalExCoeffSlider.Update( fea_mat->m_ThermalExpanCoeff.GetID() );

                m_MatE1Input.Update( fea_mat->m_E1.GetID() );
                m_MatE2Input.Update( fea_mat->m_E2.GetID() );
                m_MatE3Input.Update( fea_mat->m_E3.GetID() );
                m_Matnu12Input.Update( fea_mat->m_nu12.GetID() );
                m_Matnu13Input.Update( fea_mat->m_nu13.GetID() );
                m_Matnu23Input.Update( fea_mat->m_nu23.GetID() );
                m_MatG12Input.Update( fea_mat->m_G12.GetID() );
                m_MatG13Input.Update( fea_mat->m_G13.GetID() );
                m_MatG23Input.Update( fea_mat->m_G23.GetID() );
                m_MatA1Input.Update( fea_mat->m_A1.GetID() );
                m_MatA2Input.Update( fea_mat->m_A2.GetID() );
                m_MatA3Input.Update( fea_mat->m_A3.GetID() );

                m_FeaMaterialNameInput.Deactivate();
                m_MatDensitySlider.Deactivate();
                m_MatElasticModSlider.Deactivate();
                m_MatPoissonSlider.Deactivate();
                m_MatShearModOutput.Deactivate();
                m_MatThermalExCoeffSlider.Deactivate();
                m_MatE1Input.Deactivate();
                m_MatE2Input.Deactivate();
                m_MatE3Input.Deactivate();
                m_Matnu12Input.Deactivate();
                m_Matnu13Input.Deactivate();
                m_Matnu23Input.Deactivate();
                m_MatG12Input.Deactivate();
                m_MatG13Input.Deactivate();
                m_MatG23Input.Deactivate();
                m_MatA1Input.Deactivate();
                m_MatA2Input.Deactivate();
                m_MatA3Input.Deactivate();

                if ( fea_mat->m_UserFeaMaterial )
                {
                    m_FeaMaterialNameInput.Activate();
                    m_MatDensitySlider.Activate();
                    if ( fea_mat->m_FeaMaterialType() == vsp::FEA_ISOTROPIC )
                    {
                        m_MatElasticModSlider.Activate();
                        m_MatPoissonSlider.Activate();
                        m_MatShearModOutput.Activate();
                        m_MatThermalExCoeffSlider.Activate();
                    }
                    else
                    {
                        m_MatE1Input.Activate();
                        m_MatE2Input.Activate();
                        m_MatE3Input.Activate();
                        m_Matnu12Input.Activate();
                        m_Matnu13Input.Activate();
                        m_Matnu23Input.Activate();
                        m_MatG12Input.Activate();
                        m_MatG13Input.Activate();
                        m_MatG23Input.Activate();
                        m_MatA1Input.Activate();
                        m_MatA2Input.Activate();
                        m_MatA3Input.Activate();
                    }
                }
            }
        }


        FeaStructure* isect_struct = StructureMgr.GetFeaStruct( FeaMeshMgr.GetIntersectStructID() );
        if ( isect_struct )
        {
            if ( FeaMeshMgr.GetIntersectComplete() )
            {
                m_ExportCADButton.Activate();
            }
            else
            {
                m_ExportCADButton.Deactivate();
            }
        }
        else
        {
            m_ExportCADButton.Deactivate();
        }


        FeaMesh* mesh = FeaMeshMgr.GetMeshPtr();
        if ( mesh )
        {
            if ( mesh->m_MeshReady )
            {
                m_FeaExportFEMButton.Activate();
            }
            else
            {
                m_FeaExportFEMButton.Deactivate();
            }
        }
        else
        {
            m_FeaExportFEMButton.Deactivate();
        }



        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector< FeaStructure* > structVec = StructureMgr.GetAllFeaStructs();
            FeaStructure* curr_struct = structVec[StructureMgr.m_CurrStructIndex()];

            //==== Default Elem Size ====//
            m_MaxEdgeLen.Update( curr_struct->GetFeaGridDensityPtr()->m_BaseLen.GetID() );
            m_MinEdgeLen.Update( curr_struct->GetFeaGridDensityPtr()->m_MinLen.GetID() );
            m_MaxGap.Update( curr_struct->GetFeaGridDensityPtr()->m_MaxGap.GetID() );
            m_NumCircleSegments.Update( curr_struct->GetFeaGridDensityPtr()->m_NCircSeg.GetID() );
            m_GrowthRatio.Update( curr_struct->GetFeaGridDensityPtr()->m_GrowRatio.GetID() );
            m_Rig3dGrowthLimit.Update( curr_struct->GetFeaGridDensityPtr()->m_RigorLimit.GetID() );

            m_RelCurveTolSlider.Update( curr_struct->GetStructSettingsPtr()->m_RelCurveTol.GetID() );

            //===== Geometry Control =====//
            m_HalfMeshButton.Update( curr_struct->GetStructSettingsPtr()->m_HalfMeshFlag.GetID() );

            m_ToCubicToggle.Update( curr_struct->GetStructSettingsPtr()->m_DemoteSurfsCubicFlag.GetID() );
            m_ToCubicTolSlider.Update( curr_struct->GetStructSettingsPtr()->m_CubicSurfTolerance.GetID() );

            if ( curr_struct->GetStructSettingsPtr()->m_DemoteSurfsCubicFlag.Get() )
            {
                m_ToCubicTolSlider.Activate();
            }
            else
            {
                m_ToCubicTolSlider.Deactivate();
            }

            m_ConvertToQuadsToggle.Update( curr_struct->GetStructSettingsPtr()->m_ConvertToQuadsFlag.GetID() );
            m_HighOrderElementToggle.Update( curr_struct->GetStructSettingsPtr()->m_HighOrderElementFlag.GetID() );

            m_NodeOffset.Update( curr_struct->GetStructSettingsPtr()->m_NodeOffset.GetID() );
            m_ElementOffset.Update( curr_struct->GetStructSettingsPtr()->m_ElementOffset.GetID() );

            m_BeamPerElementNormalToggle.Update( curr_struct->GetStructSettingsPtr()->m_BeamPerElementNormal.GetID() );

            //===== Display Tab Toggle Update =====//
            m_DrawMeshButton.Update( curr_struct->GetStructSettingsPtr()->m_DrawMeshFlag.GetID() );
            m_ColorElementsButton.Update( curr_struct->GetStructSettingsPtr()->m_ColorTagsFlag.GetID() );
            m_DrawNodesToggle.Update( curr_struct->GetStructSettingsPtr()->m_DrawNodesFlag.GetID() );
            m_DrawBCNodesToggle.Update( curr_struct->GetStructSettingsPtr()->m_DrawBCNodesFlag.GetID() );
            m_DrawElementOrientVecToggle.Update( curr_struct->GetStructSettingsPtr()->m_DrawElementOrientVecFlag.GetID() );

            m_DrawIsect.Update( curr_struct->GetStructSettingsPtr()->m_DrawIsectFlag.GetID() );
            m_DrawBorder.Update( curr_struct->GetStructSettingsPtr()->m_DrawBorderFlag.GetID() );

            m_ShowRaw.Update( curr_struct->GetStructSettingsPtr()->m_DrawRawFlag.GetID() );
            m_ShowBinAdapt.Update( curr_struct->GetStructSettingsPtr()->m_DrawBinAdaptFlag.GetID() );

            m_ShowCurve.Update( curr_struct->GetStructSettingsPtr()->m_DrawCurveFlag.GetID() );
            m_ShowPts.Update( curr_struct->GetStructSettingsPtr()->m_DrawPntsFlag.GetID() );

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////
            FeaMeshMgr.UpdateDisplaySettings();
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////

            string massname = curr_struct->GetStructSettingsPtr()->GetExportFileName( vsp::FEA_MASS_FILE_NAME );
            m_MassOutput.Update( StringUtil::truncateFileName( massname, 40 ).c_str() );
            string nastranname = curr_struct->GetStructSettingsPtr()->GetExportFileName( vsp::FEA_NASTRAN_FILE_NAME );
            m_NastOutput.Update( StringUtil::truncateFileName( nastranname, 40 ).c_str() );
            string nkeyname = curr_struct->GetStructSettingsPtr()->GetExportFileName( vsp::FEA_NKEY_FILE_NAME );
            m_NkeyOutput.Update( StringUtil::truncateFileName( nkeyname, 40 ).c_str() );
            string calculixname = curr_struct->GetStructSettingsPtr()->GetExportFileName( vsp::FEA_CALCULIX_FILE_NAME );
            m_CalcOutput.Update( StringUtil::truncateFileName( calculixname, 40 ).c_str() );
            string stlname = curr_struct->GetStructSettingsPtr()->GetExportFileName( vsp::FEA_STL_FILE_NAME );
            m_StlOutput.Update( StringUtil::truncateFileName( stlname, 40 ).c_str() );
            string gmshname = curr_struct->GetStructSettingsPtr()->GetExportFileName( vsp::FEA_GMSH_FILE_NAME );
            m_GmshOutput.Update( StringUtil::truncateFileName( gmshname, 40 ).c_str() );

            //==== Update File Output Flags ====//
            m_MassFile.Update( curr_struct->GetStructSettingsPtr()->GetExportFileFlag( vsp::FEA_MASS_FILE_NAME )->GetID() );
            m_NastFile.Update( curr_struct->GetStructSettingsPtr()->GetExportFileFlag( vsp::FEA_NASTRAN_FILE_NAME )->GetID() );
            m_NkeyFile.Update( curr_struct->GetStructSettingsPtr()->GetExportFileFlag( vsp::FEA_NKEY_FILE_NAME )->GetID() );
            m_CalcFile.Update( curr_struct->GetStructSettingsPtr()->GetExportFileFlag( vsp::FEA_CALCULIX_FILE_NAME )->GetID() );
            m_StlFile.Update( curr_struct->GetStructSettingsPtr()->GetExportFileFlag( vsp::FEA_STL_FILE_NAME )->GetID() );
            m_GmshFile.Update( curr_struct->GetStructSettingsPtr()->GetExportFileFlag( vsp::FEA_GMSH_FILE_NAME )->GetID() );

            if ( !curr_struct->GetStructSettingsPtr()->GetExportFileFlag( vsp::FEA_NASTRAN_FILE_NAME )->Get() )
            {
                m_NkeyFile.Deactivate();
                m_NkeyOutput.Deactivate();
                m_SelectNkeyFile.Deactivate();
            }
            else
            {
                m_NkeyFile.Activate();
                m_NkeyOutput.Activate();
                m_SelectNkeyFile.Activate();
            }

            string srfname = curr_struct->GetStructSettingsPtr()->GetExportFileName( vsp::FEA_SRF_FILE_NAME );
            m_SrfOutput.Update( StringUtil::truncateFileName( srfname, 40 ).c_str() );

            m_SrfFile.Update( curr_struct->GetStructSettingsPtr()->GetExportFileFlag( vsp::FEA_SRF_FILE_NAME )->GetID() );
            m_XYZIntCurves.Update( curr_struct->GetStructSettingsPtr()->m_XYZIntCurveFlag.GetID() );

            string curvname = curr_struct->GetStructSettingsPtr()->GetExportFileName( vsp::FEA_CURV_FILE_NAME );
            m_CurvOutput.Update( StringUtil::truncateFileName( curvname, 40 ).c_str() );
            string plot3dname = curr_struct->GetStructSettingsPtr()->GetExportFileName( vsp::FEA_PLOT3D_FILE_NAME );
            m_Plot3DOutput.Update( StringUtil::truncateFileName( plot3dname, 40 ).c_str() );

            //==== Update File Output Flags ====//
            m_CurvFile.Update( curr_struct->GetStructSettingsPtr()->GetExportFileFlag( vsp::FEA_CURV_FILE_NAME )->GetID() );
            m_Plot3DFile.Update( curr_struct->GetStructSettingsPtr()->GetExportFileFlag( vsp::FEA_PLOT3D_FILE_NAME )->GetID() );

            m_ExportRaw.Update( curr_struct->GetStructSettingsPtr()->m_ExportRawFlag.GetID() );

            string igsname = curr_struct->GetStructSettingsPtr()->GetExportFileName( vsp::FEA_IGES_FILE_NAME );
            m_IGESOutput.Update( StringUtil::truncateFileName( igsname, 40 ).c_str() );
            m_IGESFile.Update( curr_struct->GetStructSettingsPtr()->GetExportFileFlag( vsp::FEA_IGES_FILE_NAME )->GetID() );

            string stpname = curr_struct->GetStructSettingsPtr()->GetExportFileName( vsp::FEA_STEP_FILE_NAME );
            m_STEPOutput.Update( StringUtil::truncateFileName( stpname, 40 ).c_str() );
            m_STEPFile.Update( curr_struct->GetStructSettingsPtr()->GetExportFileFlag( vsp::FEA_STEP_FILE_NAME )->GetID() );

            //m_STEPMergePointsToggle.Update( curr_struct->GetStructSettingsPtr()->m_STEPMergePoints.GetID() );
            m_STEPTolSlider.Update( curr_struct->GetStructSettingsPtr()->m_STEPTol.GetID() );
            m_STEPRepGroup.Update( curr_struct->GetStructSettingsPtr()->m_STEPRepresentation.GetID() );
            m_LenUnitChoice.Update( curr_struct->GetStructSettingsPtr()->m_CADLenUnit.GetID() );
            m_LabelIDToggle.Update( curr_struct->GetStructSettingsPtr()->m_CADLabelID.GetID() );
            m_LabelNameToggle.Update( curr_struct->GetStructSettingsPtr()->m_CADLabelName.GetID() );
            m_LabelSurfNoToggle.Update( curr_struct->GetStructSettingsPtr()->m_CADLabelSurfNo.GetID() );
            m_LabelSplitNoToggle.Update( curr_struct->GetStructSettingsPtr()->m_CADLabelSplitNo.GetID() );
            m_LabelDelimChoice.Update( curr_struct->GetStructSettingsPtr()->m_CADLabelDelim.GetID() );

            string test1 = to_string( curr_struct->GetStructSettingsPtr()->GetExportFileFlag( vsp::FEA_STEP_FILE_NAME )->Get() );
            string test2 = to_string( curr_struct->GetStructSettingsPtr()->GetExportFileFlag( vsp::FEA_IGES_FILE_NAME )->Get() );


            if ( !curr_struct->GetStructSettingsPtr()->GetExportFileFlag( vsp::FEA_STEP_FILE_NAME )->Get() )
            {
                //m_STEPMergePointsToggle.Deactivate();
                m_STEPTolSlider.Deactivate();
                m_STEPRepGroup.Deactivate();
            }
            else
            {
                //m_STEPMergePointsToggle.Activate();
                m_STEPTolSlider.Activate();
                m_STEPRepGroup.Activate();
            }

            if ( !curr_struct->GetStructSettingsPtr()->GetExportFileFlag( vsp::FEA_STEP_FILE_NAME )->Get() &&
                 !curr_struct->GetStructSettingsPtr()->GetExportFileFlag( vsp::FEA_IGES_FILE_NAME )->Get() )
            {
                m_LabelIDToggle.Deactivate();
                m_LabelNameToggle.Deactivate();
                m_LabelSurfNoToggle.Deactivate();
                m_LenUnitChoice.Deactivate();
                m_LabelSplitNoToggle.Deactivate();
                m_LabelDelimChoice.Deactivate();
            }
            else
            {
                m_LabelIDToggle.Activate();
                m_LabelNameToggle.Activate();
                m_LabelSurfNoToggle.Activate();
                m_LenUnitChoice.Activate();
                m_LabelSplitNoToggle.Activate();
                m_LabelDelimChoice.Activate();
            }
            
            // Update Structure Name
            m_FeaStructNameInput.Update( curr_struct->GetName() );

            if ( m_SelectedPartIndexVec.size() > 0 )
            {
                FeaStructDispGroup( &m_PartGroup );
                m_FeaPartNameInput.Deactivate();
                m_EditFeaPartButton.Deactivate();
                m_DispFeaPartGroup.Activate();
            }
            else
            {
                FeaStructDispGroup( NULL );
            }

            if ( m_SelectedPartIndexVec.size() == 1 )
            {
                m_FeaPartNameInput.Activate();
                m_EditFeaPartButton.Activate();

                if ( m_SelectedPartIndexVec[0] < curr_struct->NumFeaParts() )
                {
                    FeaPart* prt = curr_struct->GetFeaPart( m_SelectedPartIndexVec[0] );

                    if ( prt )
                    {
                        m_FeaPartNameInput.Update( prt->GetName() );
                        m_DispFeaPartGroup.Update( prt->m_DrawFeaPartFlag.GetID() );
                        m_DispFeaPartGroup.Activate();

                        if ( prt->GetType() == vsp::FEA_FIX_POINT )
                        {
                        }
                        else if ( prt->GetType() == vsp::FEA_SKIN )
                        {
                            m_DispFeaPartGroup.Deactivate();
                        }
                    }
                    else
                    {
                        FeaStructDispGroup( NULL );
                    }
                }
                else if ( m_SelectedPartIndexVec[0] >= curr_struct->NumFeaParts() )
                {
                    SubSurface* subsurf = curr_struct->GetFeaSubSurf( m_SelectedPartIndexVec[0] - curr_struct->NumFeaParts() );

                    if ( subsurf )
                    {
                        m_FeaPartNameInput.Update( subsurf->GetName() );
                        m_DispFeaPartGroup.Update( subsurf->m_DrawFeaPartFlag.GetID() );
                    }
                    else
                    {
                        FeaStructDispGroup( NULL );
                    }
                }
                else
                {
                    FeaStructDispGroup( NULL );
                }
            }

        }

        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure * > structVec = StructureMgr.GetAllFeaStructs();
            FeaStructure *curr_struct = structVec[ StructureMgr.m_CurrStructIndex() ];

            UpdateBCPartChoice();
            UpdateBCSubSurfChoice();
            UpdateFeaBCBrowser();

            if ( curr_struct->ValidFeaBCInd( StructureMgr.GetCurrBCIndex() ) )
            {
                FeaBC *curr_bc = curr_struct->GetFeaBC( StructureMgr.GetCurrBCIndex() );

                m_FeaBCTypeChoice.Update( curr_bc->m_FeaBCType.GetID() );

                m_FeaBCModeChoice.Update( curr_bc->m_ConMode.GetID() );

                m_TxButton.Update( curr_bc->m_Constraints.GetID() );
                m_TyButton.Update( curr_bc->m_Constraints.GetID() );
                m_TzButton.Update( curr_bc->m_Constraints.GetID() );
                m_RxButton.Update( curr_bc->m_Constraints.GetID() );
                m_RyButton.Update( curr_bc->m_Constraints.GetID() );
                m_RzButton.Update( curr_bc->m_Constraints.GetID() );

                m_FeaBCPartChoice.Deactivate();
                m_FeaBCSubSurfChoice.Deactivate();

                if ( curr_bc->m_FeaBCType() == vsp::FEA_BC_PART )
                {
                    m_FeaBCPartChoice.Activate();
                    m_SelectedBCPartChoice = vector_find_val( m_FeaBCPartChoiceIDVec, curr_bc->GetPartID() );
                    m_SelectedBCSubSurfChoice = -1;
                }
                else if ( curr_bc->m_FeaBCType() == vsp::FEA_BC_SUBSURF )
                {
                    m_FeaBCSubSurfChoice.Activate();
                    m_SelectedBCPartChoice = -1;
                    m_SelectedBCSubSurfChoice = vector_find_val( m_FeaBCSubSurfChoiceIDVec, curr_bc->GetSubSurfID() );
                }
                else
                {
                    m_SelectedBCPartChoice = -1;
                    m_SelectedBCSubSurfChoice = -1;
                }
                m_FeaBCPartChoice.SetVal( m_SelectedBCPartChoice );
                m_FeaBCSubSurfChoice.SetVal( m_SelectedBCSubSurfChoice );


                m_XLTFlagButton.Update( curr_bc->m_XLTFlag.GetID() );
                m_XGTFlagButton.Update( curr_bc->m_XGTFlag.GetID() );

                m_YLTFlagButton.Update( curr_bc->m_YLTFlag.GetID() );
                m_YGTFlagButton.Update( curr_bc->m_YGTFlag.GetID() );

                m_ZLTFlagButton.Update( curr_bc->m_ZLTFlag.GetID() );
                m_ZGTFlagButton.Update( curr_bc->m_ZGTFlag.GetID() );

                m_XLTValSlider.Update( curr_bc->m_XLTVal.GetID() );
                m_XGTValSlider.Update( curr_bc->m_XGTVal.GetID() );

                m_YLTValSlider.Update( curr_bc->m_YLTVal.GetID() );
                m_YGTValSlider.Update( curr_bc->m_YGTVal.GetID() );

                m_ZLTValSlider.Update( curr_bc->m_ZLTVal.GetID() );
                m_ZGTValSlider.Update( curr_bc->m_ZGTVal.GetID() );

                m_XLTValSlider.Deactivate();
                m_XGTValSlider.Deactivate();
                m_YLTValSlider.Deactivate();
                m_YGTValSlider.Deactivate();
                m_ZLTValSlider.Deactivate();
                m_ZGTValSlider.Deactivate();

                if ( curr_bc->m_XLTFlag() )
                {
                    m_XLTValSlider.Activate();
                }
                if ( curr_bc->m_XGTFlag() )
                {
                    m_XGTValSlider.Activate();
                }

                if ( curr_bc->m_YLTFlag() )
                {
                    m_YLTValSlider.Activate();
                }
                if ( curr_bc->m_YGTFlag() )
                {
                    m_YGTValSlider.Activate();
                }

                if ( curr_bc->m_ZLTFlag() )
                {
                    m_ZLTValSlider.Activate();
                }
                if ( curr_bc->m_ZGTFlag() )
                {
                    m_ZGTValSlider.Activate();
                }

            }
        }

    }

    //If size is > 1 then a Structure has been added to Browser, and we activate export buttons
    if ( ( int )( m_FeaPartSelectBrowser->size() > 1 ) )
    {
        m_IntersectOnlyButton.Activate();
        m_FeaIntersectMeshButton.Activate();
    }
    else
    {
        m_IntersectOnlyButton.Deactivate();
        m_FeaIntersectMeshButton.Deactivate();
    }

    return true;
}

void StructScreen::AddOutputText( const string &text )
{
    Fl::lock();
    m_ConsoleBuffer->append( text.c_str() );
    m_ConsoleDisplay->insert_position( m_ConsoleDisplay->buffer()->length() );
    m_ConsoleDisplay->show_insert_position();
    Fl::unlock();
}

void StructScreen::CallBack( Fl_Widget* w )
{
    assert( m_ScreenMgr );

    Vehicle*  veh = m_ScreenMgr->GetVehiclePtr();

    if ( veh )
    {
        if ( w == m_FeaPartSelectBrowser )
        {
            if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
            {
                vector< FeaStructure* > structVec = StructureMgr.GetAllFeaStructs();

                m_SelectedPartIndexVec.clear();

                for ( size_t i = 2; i <= m_FeaPartSelectBrowser->size(); i++ )
                {
                    if ( m_FeaPartSelectBrowser->selected( i ) )
                    {
                        m_SelectedPartIndexVec.push_back( i - 2 );
                    }
                }

                if ( m_SelectedPartIndexVec.size() == 1 )
                {
                    StructureMgr.SetCurrPartIndex( m_SelectedPartIndexVec[0] );

                    if ( m_SelectedPartIndexVec[0] < structVec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() )
                    {
                        FeaPart* feaprt = structVec[StructureMgr.m_CurrStructIndex()]->GetFeaPart( m_SelectedPartIndexVec[0] );

                        if ( feaprt )
                        {
                            if ( feaprt->GetType() == vsp::FEA_SKIN )
                            {
                                m_SelectedFeaPartChoice = 0; // No dropdown available
                            }
                            else
                            {
                                m_SelectedFeaPartChoice = feaprt->GetType();
                            }
                        }
                    }
                    else if ( m_SelectedPartIndexVec[0] >= structVec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() )
                    {
                        SubSurface* subsurf = structVec[StructureMgr.m_CurrStructIndex()]->GetFeaSubSurf( m_SelectedPartIndexVec[0] - structVec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() );

                        if ( subsurf )
                        {
                            m_SelectedFeaPartChoice = subsurf->GetType() + m_FeaPartChoiceSubSurfOffset;
                        }
                    }
                }
                else
                {
                    StructureMgr.SetCurrPartIndex( -1 );
                }

                if ( Fl::event_clicks() != 0 ) // Indicates a double click
                {
                    if ( m_SelectedPartIndexVec.size() == 1 )
                    {
                        StructureMgr.SetCurrPartIndex( m_SelectedPartIndexVec[0] );
                        m_ScreenMgr->ShowScreen( ScreenMgr::VSP_FEA_PART_EDIT_SCREEN );
                    }
                }
            }
        }
        else if ( w == m_StructureSelectBrowser )
        {
            int indx = m_StructureSelectBrowser->value() - 2;
            if ( StructureMgr.ValidTotalFeaStructInd( indx  ) )
            {
                StructureMgr.m_CurrStructIndex.Set( indx );
                FeaMeshMgr.SetFeaMeshStructID( m_StructIDs[ StructureMgr.m_CurrStructIndex() ] );
            }
        }
        else if ( w == m_DrawPartSelectBrowser )
        {
            int selected_index = m_DrawPartSelectBrowser->value();
            bool flag = !!m_DrawPartSelectBrowser->checked( selected_index );
            selected_index--;

            vector < string > draw_browser_name_vec = FeaMeshMgr.GetMeshPtr()->GetDrawBrowserNameVec();
            vector < int > draw_browser_index_vec = FeaMeshMgr.GetMeshPtr()->GetDrawBrowserIndexVec();

            if ( selected_index < draw_browser_name_vec.size() )
            {
                if ( draw_browser_name_vec[ selected_index ].find( "CAP" ) != std::string::npos )
                {
                    FeaMeshMgr.GetMeshPtr()->SetDrawCapFlag( draw_browser_index_vec[ selected_index ], flag );
                }
                else
                {
                    FeaMeshMgr.GetMeshPtr()->SetDrawElementFlag( draw_browser_index_vec[ selected_index ], flag );
                }
            }
        }
        else if ( w == m_FeaPropertySelectBrowser )
        {
            StructureMgr.SetCurrPropertyIndex( m_FeaPropertySelectBrowser->value() - 2 );
        }
        else if ( w == m_FeaMaterialSelectBrowser )
        {
            StructureMgr.SetCurrMaterialIndex( m_FeaMaterialSelectBrowser->value() - 1 );
        }
        else if ( w == m_FeaBCSelectBrowser )
        {
            StructureMgr.SetCurrBCIndex( m_FeaBCSelectBrowser->value() - 1 );
        }
    }

    m_ScreenMgr->SetUpdateFlag( true );
}

#ifdef WIN32
DWORD WINAPI feamesh_thread_fun( LPVOID data )
#else
void * feamesh_thread_fun( void *data )
#endif
{
    FeaMeshMgr.GenerateFeaMesh();

    return 0;
}

void StructScreen::LaunchFEAMesh()
{
    if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
    {
        // Set m_FeaMeshInProgress to ensure m_MonitorProcess does not terminate prematurely
        FeaMeshMgr.SetFeaMeshInProgress( true );

        m_ConsoleBuffer->text( "" );

        // Identify which structure to mesh
        FeaMeshMgr.SetFeaMeshStructID( m_StructIDs[ StructureMgr.m_CurrStructIndex() ] );

        m_FeaMeshProcess.StartThread( feamesh_thread_fun, NULL );

        vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

        structvec[StructureMgr.m_CurrStructIndex()]->GetStructSettingsPtr()->m_DrawMeshFlag = true;
        structvec[StructureMgr.m_CurrStructIndex()]->SetDrawFlag( false );
    }
}

void StructScreen::GuiDeviceCallBack( GuiDevice* device )
{
    assert( m_ScreenMgr );

    Vehicle*  veh = m_ScreenMgr->GetVehiclePtr();

    if ( !veh )
    {
        return;
    }

    if ( device == &m_FeaIntersectMeshButton )
    {
        LaunchFEAMesh();
    }
    else if ( device == &m_FeaExportFEMButton )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            FeaMeshMgr.addOutputText( "Exporting Mesh Files\n" );
            FeaMeshMgr.ExportFeaMesh( m_StructIDs[ StructureMgr.m_CurrStructIndex() ] );
        }
    }
    else if ( device == &m_IntersectOnlyButton )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            // Set m_FeaMeshInProgress to ensure m_MonitorProcess does not terminate prematurely
            FeaMeshMgr.SetFeaMeshInProgress( true );
            FeaMeshMgr.SetCADOnlyFlag( true );

            // Identify which structure to mesh
            FeaMeshMgr.SetFeaMeshStructID( m_StructIDs[ StructureMgr.m_CurrStructIndex() ] );

            m_FeaMeshProcess.StartThread( feamesh_thread_fun, NULL );
        }
    }
    else if ( device == &m_ExportCADButton )
    {
        FeaMeshMgr.addOutputText( "Exporting CAD Files\n" );
        FeaMeshMgr.ExportCADFiles();
    }
    else if ( device == &m_ResetPartDisplayButton )
    {
        StructureMgr.ShowAllParts();

        if ( FeaMeshMgr.GetMeshPtr() )
        {
            FeaMeshMgr.GetMeshPtr()->SetAllDisplayFlags( false );
        }
    }
    else if ( device == &m_ResetMeshDisplayButton )
    {
        StructureMgr.HideAllParts();

        if ( FeaMeshMgr.GetMeshPtr() )
        {
            FeaMeshMgr.GetMeshPtr()->SetAllDisplayFlags( true );
        }
    }
    else if ( device == &m_WikiLinkButton )
    {
#if     defined(__APPLE__)
        system( "open http://www.openvsp.org/wiki/doku.php?id=feamesh" );
#elif   defined(_WIN32) || defined(WIN32) 
        ShellExecute( NULL, "open", "http://www.openvsp.org/wiki/doku.php?id=feamesh",
                      NULL, NULL, SW_SHOWNORMAL );
#else
        system( "xdg-open http://www.openvsp.org/wiki/doku.php?id=feamesh" );
#endif
    }
    else if ( device == &m_StructUnitChoice )
    {
        StructureMgr.UpdateStructUnit( m_StructUnitChoice.GetVal() );
    }
    else if ( device == &m_GeomChoice )
    {
        m_SelectedGeomID = m_GeomIDVec[m_GeomChoice.GetVal()];
    }
    else if ( device == &m_SurfSel )
    {
        m_FeaCurrMainSurfIndx = m_SurfSel.GetVal();
    }
    else if ( device == &m_AddFeaStructButton )
    {
        Geom* currgeom = veh->FindGeom( m_SelectedGeomID );
        if ( currgeom )
        {
            FeaStructure* newstruct = currgeom->AddFeaStruct( true, m_FeaCurrMainSurfIndx );

            if ( newstruct )
            {
                StructureMgr.InitFeaProperties(); // Add default FeaProperties if none available

                StructureMgr.m_CurrStructIndex.Set( StructureMgr.GetTotFeaStructIndex( newstruct ) );
            }
        }
    }
    else if ( device == &m_DelFeaStructButton )
    {
        vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            FeaStructure* delstruct = structvec[StructureMgr.m_CurrStructIndex()];

            if ( delstruct )
            {
                Geom* currgeom = veh->FindGeom( delstruct->GetParentGeomID() );

                if ( currgeom )
                {
                    vector < FeaStructure* > geomstructvec = currgeom->GetFeaStructVec();

                    for ( unsigned int i = 0; i < geomstructvec.size(); i++ )
                    {
                        if ( geomstructvec[i] == delstruct )
                        {
                            currgeom->DeleteFeaStruct( i );
                        }
                    }
                }
            }
        }

        StructureMgr.m_CurrStructIndex.Set( StructureMgr.m_CurrStructIndex() - 1 );

        if ( !StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            StructureMgr.m_CurrStructIndex.Set( -1 );
        }
    }
    else if ( device == &m_FeaStructNameInput )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            FeaStructure* feastruct = structvec[StructureMgr.m_CurrStructIndex()];

            if ( feastruct )
            {
                feastruct->SetName( m_FeaStructNameInput.GetString() );

                if ( feastruct->GetStructSettingsPtr() )
                {
                    feastruct->ResetExportFileNames();
                }
            }
        }
    }
    else if ( device == &m_OrientWingButton )
    {
        OrientWing();
    }
    else if ( device == &m_OrientFrontButton )
    {
        OrientStructure( VSPGraphic::Common::VSP_CAM_FRONT );
    }
    else if ( device == &m_OrientSideButton )
    {
        OrientStructure( VSPGraphic::Common::VSP_CAM_LEFT );
    }
    else if ( device == &m_OrientTopButton )
    {
        OrientStructure( VSPGraphic::Common::VSP_CAM_TOP );
    }
    else if ( device == &m_FeaPartNameInput )
    {
        vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

        if ( m_SelectedPartIndexVec.size() == 1 )
        {
            if ( m_SelectedPartIndexVec[0] < structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() )
            {
                FeaPart* feaprt = structvec[StructureMgr.m_CurrStructIndex()]->GetFeaPart( m_SelectedPartIndexVec[0] );

                if ( feaprt )
                {
                    feaprt->SetName( m_FeaPartNameInput.GetString() );
                }
            }
            else if ( m_SelectedPartIndexVec[0] >= structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() )
            {
                SubSurface* subsurf = structvec[StructureMgr.m_CurrStructIndex()]->GetFeaSubSurf( m_SelectedPartIndexVec[0] - structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() );

                if ( subsurf )
                {
                    subsurf->SetName( m_FeaPartNameInput.GetString() );
                }
            }
        }
    }
    else if ( device == &m_EditFeaPartButton )
    {
        if ( m_SelectedPartIndexVec.size() == 1 )
        {
            StructureMgr.SetCurrPartIndex( m_SelectedPartIndexVec[0] );

            m_ScreenMgr->ShowScreen( ScreenMgr::VSP_FEA_PART_EDIT_SCREEN );
        }
        else
        {
            AddOutputText( "Error: Select A Single FEA Part\n" );
        }
    }
    else if ( device == &m_FeaPartChoice )
    {
        m_SelectedFeaPartChoice = m_FeaPartChoice.GetVal();
    }
    else if ( device == &m_AddFeaPartButton )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            m_SelectedPartIndexVec.clear();

            if ( m_FeaPartChoice.GetVal() < m_FeaPartChoiceSubSurfOffset )
            {
                FeaPart* feaprt = NULL;

                feaprt = structvec[StructureMgr.m_CurrStructIndex()]->AddFeaPart( m_FeaPartChoice.GetVal() );

                if ( m_FeaPartChoice.GetVal() == vsp::FEA_SLICE )
                {
                    FeaSlice* slice = dynamic_cast<FeaSlice*>( feaprt );
                    assert( slice );

                    slice->m_OrientationPlane.Set( StructureMgr.GetFeaSliceOrientIndex() );
                }

                if ( feaprt )
                {
                    m_SelectedPartIndexVec.push_back( structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() - 1 );
                    StructureMgr.SetCurrPartIndex( structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() - 1 );
                    feaprt->Update();
                }
            }
            else
            {
                SubSurface* ssurf = NULL;

                ssurf = structvec[StructureMgr.m_CurrStructIndex()]->AddFeaSubSurf( m_FeaPartChoice.GetVal() - m_FeaPartChoiceSubSurfOffset );

                if ( ssurf )
                {
                    m_SelectedPartIndexVec.push_back( structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() + structvec[StructureMgr.m_CurrStructIndex()]->NumFeaSubSurfs() - 1 );
                    StructureMgr.SetCurrPartIndex( structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() + structvec[StructureMgr.m_CurrStructIndex()]->NumFeaSubSurfs() - 1 );
                    ssurf->Update();
                }
            }
        }
        else
        {
            AddOutputText( "Error: No Structure Selected\n" );
        }
    }
    else if ( device == &m_DelFeaPartButton )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            if ( m_SelectedPartIndexVec.size() > 0 )
            {
                for ( size_t i = 0; i < m_SelectedPartIndexVec.size(); i++ )
                {
                    FeaPart* prt = structvec[StructureMgr.m_CurrStructIndex()]->GetFeaPart( m_SelectedPartIndexVec[i] );

                    if ( prt )
                    {
                        if ( prt->GetType() == vsp::FEA_SKIN )
                        {
                            m_SelectedPartIndexVec.erase( m_SelectedPartIndexVec.begin() + i ); // Do not include FeaSkin
                            break;
                        }
                    }
                }

                int first_selection = -1;
                if ( m_SelectedPartIndexVec.size() > 0 )
                {
                    first_selection = m_SelectedPartIndexVec[0];
                }

                while ( m_SelectedPartIndexVec.size() > 0 )
                {
                    if ( m_SelectedPartIndexVec[0] < structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() )
                    {
                        structvec[StructureMgr.m_CurrStructIndex()]->DelFeaPart( m_SelectedPartIndexVec[0] );
                    }
                    else if ( m_SelectedPartIndexVec[0] >= structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() )
                    {
                        structvec[StructureMgr.m_CurrStructIndex()]->DelFeaSubSurf( m_SelectedPartIndexVec[0] - structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() );
                    }

                    vector < int > temp_index_vec;

                    for ( size_t j = 1; j < m_SelectedPartIndexVec.size(); j++ )
                    {
                        temp_index_vec.push_back( m_SelectedPartIndexVec[j] - 1 );
                    }

                    m_SelectedPartIndexVec = temp_index_vec;
                }

                if ( first_selection != -1 )
                {
                    if ( structvec[StructureMgr.m_CurrStructIndex()]->ValidFeaPartInd( first_selection - 1 ) )
                    {
                        m_SelectedPartIndexVec.clear();
                        m_SelectedPartIndexVec.push_back( first_selection - 1 );
                        StructureMgr.SetCurrPartIndex( m_SelectedPartIndexVec[0] );
                    }
                }
            }
        }
    }
    else if ( device == &m_DispFeaPartGroup )
    {
        string curr_parm_id = m_DispFeaPartGroup.GetParmID();
        Parm* curr_parm = ParmMgr.FindParm( curr_parm_id );
        bool curr_val = curr_parm->Get();

        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) && m_SelectedPartIndexVec.size() > 1 )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            for ( size_t i = 0; i < m_SelectedPartIndexVec.size(); i++ )
            {
                if ( m_SelectedPartIndexVec[i] < structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() )
                {
                    FeaPart* prt = structvec[StructureMgr.m_CurrStructIndex()]->GetFeaPart( m_SelectedPartIndexVec[i] );

                    if ( prt )
                    {
                        if ( prt->GetType() != vsp::FEA_SKIN )
                        {
                            prt->m_DrawFeaPartFlag.Set( curr_val );
                        }
                        else
                        {
                            prt->m_DrawFeaPartFlag.Set( true );
                        }
                    }
                }
                else if ( m_SelectedPartIndexVec[i] >= structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() )
                {
                    SubSurface* ssurf = structvec[StructureMgr.m_CurrStructIndex()]->GetFeaSubSurf( m_SelectedPartIndexVec[i] - structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() );

                    if ( ssurf )
                    {
                        ssurf->m_DrawFeaPartFlag.Set( curr_val );
                    }
                }
            }
        }
    }
    else if ( device == &m_DrawAllButton )
    {
        FeaMeshMgr.GetMeshPtr()->SetAllDisplayFlags( true );
    }
    else if ( device == &m_HideAllButton )
    {
        FeaMeshMgr.GetMeshPtr()->SetAllDisplayFlags( false );
    }
    else if ( device == &m_MovePrtUpButton )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) && m_SelectedPartIndexVec.size() == 1 )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            if ( m_SelectedPartIndexVec[0] < structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() )
            {
                structvec[StructureMgr.m_CurrStructIndex()]->ReorderFeaPart( m_SelectedPartIndexVec[0], Vehicle::REORDER_MOVE_UP );

                if ( m_SelectedPartIndexVec[0] != 0 )
                {
                    m_SelectedPartIndexVec[0]--;
                    StructureMgr.SetCurrPartIndex( m_SelectedPartIndexVec[0] );
                }
            }
            else if ( m_SelectedPartIndexVec[0] >= structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() )
            {
                structvec[StructureMgr.m_CurrStructIndex()]->ReorderFeaSubSurf( m_SelectedPartIndexVec[0] - structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts(), Vehicle::REORDER_MOVE_UP );

                if ( m_SelectedPartIndexVec[0] != structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() )
                {
                    m_SelectedPartIndexVec[0]--;
                    StructureMgr.SetCurrPartIndex( m_SelectedPartIndexVec[0] );
                }
            }
        }
    }
    else if ( device == &m_MovePrtDownButton )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) && m_SelectedPartIndexVec.size() == 1 )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            if ( m_SelectedPartIndexVec[0] < structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() )
            {
                structvec[StructureMgr.m_CurrStructIndex()]->ReorderFeaPart( m_SelectedPartIndexVec[0], Vehicle::REORDER_MOVE_DOWN );

                if ( m_SelectedPartIndexVec[0] != structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() - 1 )
                {
                    m_SelectedPartIndexVec[0]++;
                    StructureMgr.SetCurrPartIndex( m_SelectedPartIndexVec[0] );
                }
            }
            else if ( m_SelectedPartIndexVec[0] >= structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() )
            {
                structvec[StructureMgr.m_CurrStructIndex()]->ReorderFeaSubSurf( m_SelectedPartIndexVec[0] - structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts(), Vehicle::REORDER_MOVE_DOWN );

                if ( m_SelectedPartIndexVec[0] != structvec[StructureMgr.m_CurrStructIndex()]->NumFeaSubSurfs() + structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() - 1 )
                {
                    m_SelectedPartIndexVec[0]++;
                    StructureMgr.SetCurrPartIndex( m_SelectedPartIndexVec[0] );
                }
            }
        }
    }
    else if ( device == &m_MovePrtTopButton )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) && m_SelectedPartIndexVec.size() == 1 )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            if ( m_SelectedPartIndexVec[0] < structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() )
            {
                structvec[StructureMgr.m_CurrStructIndex()]->ReorderFeaPart( m_SelectedPartIndexVec[0], Vehicle::REORDER_MOVE_TOP );
                m_SelectedPartIndexVec[0] = 0;
                StructureMgr.SetCurrPartIndex( m_SelectedPartIndexVec[0] );
            }
            else if ( m_SelectedPartIndexVec[0] >= structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() )
            {
                structvec[StructureMgr.m_CurrStructIndex()]->ReorderFeaSubSurf( m_SelectedPartIndexVec[0] - structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts(), Vehicle::REORDER_MOVE_TOP );
                m_SelectedPartIndexVec[0] = structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts();
                StructureMgr.SetCurrPartIndex( m_SelectedPartIndexVec[0] );
            }
        }
    }
    else if ( device == &m_MovePrtBotButton )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) && m_SelectedPartIndexVec.size() == 1 )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            if ( m_SelectedPartIndexVec[0] < structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() )
            {
                structvec[StructureMgr.m_CurrStructIndex()]->ReorderFeaPart( m_SelectedPartIndexVec[0], Vehicle::REORDER_MOVE_BOTTOM );
                m_SelectedPartIndexVec[0] = structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() - 1;
                StructureMgr.SetCurrPartIndex( m_SelectedPartIndexVec[0] );
            }
            else if ( m_SelectedPartIndexVec[0] >= structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() )
            {
                structvec[StructureMgr.m_CurrStructIndex()]->ReorderFeaSubSurf( m_SelectedPartIndexVec[0] - structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts(), Vehicle::REORDER_MOVE_BOTTOM );
                m_SelectedPartIndexVec[0] = structvec[StructureMgr.m_CurrStructIndex()]->NumFeaParts() + structvec[StructureMgr.m_CurrStructIndex()]->NumFeaSubSurfs() - 1;
                StructureMgr.SetCurrPartIndex( m_SelectedPartIndexVec[0] );
            }
        }
    }
    else if ( device == &m_AddFeaPropertyButton )
    {
        StructureMgr.AddFeaProperty( m_FeaPropertyType.GetVal() );
        StructureMgr.SetCurrPropertyIndex( StructureMgr.NumFeaProperties() - 1 );
    }
    else if ( device == &m_DelFeaPropertyButton )
    {
        if ( StructureMgr.ValidFeaPropertyInd( StructureMgr.GetCurrPropertyIndex() ) )
        {
            StructureMgr.DeleteFeaProperty( StructureMgr.GetCurrPropertyIndex() );

            StructureMgr.SetCurrPropertyIndex( StructureMgr.GetCurrPropertyIndex() - 1 );
        }
        else
        {
            StructureMgr.SetCurrPropertyIndex( -1 );
        }
    }
    else if ( device == &m_FeaPropertyNameInput )
    {
        if ( StructureMgr.ValidFeaPropertyInd( StructureMgr.GetCurrPropertyIndex() ) )
        {
            vector < FeaProperty* > property_vec = StructureMgr.GetFeaPropertyVec();
            FeaProperty* fea_prop = property_vec[StructureMgr.GetCurrPropertyIndex()];

            if ( fea_prop )
            {
                fea_prop->SetName( m_FeaPropertyNameInput.GetString() );
            }
        }
    }
    else if ( device == &m_AddFeaMaterialButton )
    {
        StructureMgr.AddFeaMaterial();
        StructureMgr.SetCurrMaterialIndex( StructureMgr.NumFeaMaterials() - 1 );
    }
    else if ( device == &m_DelFeaMaterialButton )
    {
        if ( StructureMgr.ValidFeaMaterialInd( StructureMgr.GetCurrMaterialIndex() ) )
        {
            StructureMgr.DeleteFeaMaterial( StructureMgr.GetCurrMaterialIndex() );

            StructureMgr.SetCurrMaterialIndex( StructureMgr.GetCurrMaterialIndex() - 1 );
        }
        else
        {
            StructureMgr.SetCurrMaterialIndex( -1 );
        }
    }
    else if ( device == &m_FeaMaterialNameInput )
    {
        if ( StructureMgr.ValidFeaMaterialInd( StructureMgr.GetCurrMaterialIndex() ) )
        {
            vector < FeaMaterial* > material_vec = StructureMgr.GetFeaMaterialVec();
            FeaMaterial* fea_mat = material_vec[StructureMgr.GetCurrMaterialIndex()];

            if ( fea_mat )
            {
                fea_mat->SetName( m_FeaMaterialNameInput.GetString() );
            }
        }
    }
    else if ( device == &m_FeaShellMaterialChoice )
    {
        if ( StructureMgr.ValidFeaPropertyInd( StructureMgr.GetCurrPropertyIndex() ) )
        {
            FeaProperty* fea_prop = StructureMgr.GetFeaPropertyVec()[StructureMgr.GetCurrPropertyIndex()];

            if ( fea_prop )
            {
                fea_prop->m_FeaMaterialIndex.Set( m_FeaShellMaterialChoice.GetVal() );
            }
        }
    }
    else if ( device == &m_FeaBeamMaterialChoice )
    {
        if ( StructureMgr.ValidFeaPropertyInd( StructureMgr.GetCurrPropertyIndex() ) )
        {
            FeaProperty* fea_prop = StructureMgr.GetFeaPropertyVec()[StructureMgr.GetCurrPropertyIndex()];

            if ( fea_prop )
            {
                fea_prop->m_FeaMaterialIndex.Set( m_FeaBeamMaterialChoice.GetVal() );
            }
        }
    }
    else if ( device == &m_ShowFeaBeamXSecButton )
    {
        // Show XSec Diagram with Dimensions
        m_ScreenMgr->ShowScreen( ScreenMgr::VSP_FEA_XSEC_SCREEN );
    }
    else if ( device == &m_SelectStlFile )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            string newfile = m_ScreenMgr->GetSelectFileScreen()->FileChooser( "Select .stl file.", "*.stl" );
            if ( newfile.compare( "" ) != 0 )
            {
                structvec[StructureMgr.m_CurrStructIndex()]->GetStructSettingsPtr()->SetExportFileName( newfile, vsp::FEA_STL_FILE_NAME );
            }
        }
    }
    else if ( device == &m_SelectMassFile )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            string newfile = m_ScreenMgr->GetSelectFileScreen()->FileChooser( "Select mass .txt file.", "*.txt" );
            if ( newfile.compare( "" ) != 0 )
            {
                structvec[StructureMgr.m_CurrStructIndex()]->GetStructSettingsPtr()->SetExportFileName( newfile, vsp::FEA_MASS_FILE_NAME );
            }
        }
    }
    else if ( device == &m_SelectNastFile )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            string newfile = m_ScreenMgr->GetSelectFileScreen()->FileChooser( "Select NASTRAN .dat file.", "*.dat" );
            if ( newfile.compare( "" ) != 0 )
            {
                structvec[StructureMgr.m_CurrStructIndex()]->GetStructSettingsPtr()->SetExportFileName( newfile, vsp::FEA_NASTRAN_FILE_NAME );
            }
        }
    }
    else if ( device == &m_SelectNkeyFile )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            string newfile = m_ScreenMgr->GetSelectFileScreen()->FileChooser( "Select NASTRAN key file.", "*.nkey" );
            if ( newfile.compare( "" ) != 0 )
            {
                structvec[StructureMgr.m_CurrStructIndex()]->GetStructSettingsPtr()->SetExportFileName( newfile, vsp::FEA_NKEY_FILE_NAME );
            }
        }
    }
    else if ( device == &m_SelectCalcFile  )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            string newfile = m_ScreenMgr->GetSelectFileScreen()->FileChooser( "Select Calculix .dat file.", "*.dat" );
            if ( newfile.compare( "" ) != 0 )
            {
                structvec[StructureMgr.m_CurrStructIndex()]->GetStructSettingsPtr()->SetExportFileName( newfile, vsp::FEA_CALCULIX_FILE_NAME );
            }
        }
    }
    else if ( device == &m_SelectGmshFile )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            string newfile = m_ScreenMgr->GetSelectFileScreen()->FileChooser( "Select .msh file.", "*.msh" );
            if ( newfile.compare( "" ) != 0 )
            {
                structvec[StructureMgr.m_CurrStructIndex()]->GetStructSettingsPtr()->SetExportFileName( newfile, vsp::FEA_GMSH_FILE_NAME );
            }
        }
    }
    else if ( device == &m_SelectSrfFile )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            string newfile = m_ScreenMgr->GetSelectFileScreen()->FileChooser( "Select .srf file.", "*.srf" );
            if ( newfile.compare( "" ) != 0 )
            {
                structvec[StructureMgr.m_CurrStructIndex()]->GetStructSettingsPtr()->SetExportFileName( newfile, vsp::FEA_SRF_FILE_NAME );
            }
        }
    }
    else if ( device == &m_SelectCurvFile )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            string newfile = m_ScreenMgr->GetSelectFileScreen()->FileChooser( "Select GridTool .curv file.", "*.curv" );
            if ( newfile.compare( "" ) != 0 )
            {
                structvec[StructureMgr.m_CurrStructIndex()]->GetStructSettingsPtr()->SetExportFileName( newfile, vsp::FEA_CURV_FILE_NAME );
            }
        }
    }
    else if ( device == &m_SelectPlot3DFile )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            string newfile = m_ScreenMgr->GetSelectFileScreen()->FileChooser( "Select Plot3D .p3d file.", "*.p3d" );
            if ( newfile.compare( "" ) != 0 )
            {
                structvec[StructureMgr.m_CurrStructIndex()]->GetStructSettingsPtr()->SetExportFileName( newfile, vsp::FEA_PLOT3D_FILE_NAME );
            }
        }
    }
    else if ( device == &m_SelectIGESFile )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            string newfile = m_ScreenMgr->GetSelectFileScreen()->FileChooser( "Select IGES .igs file.", "*.igs" );
            if ( newfile.compare( "" ) != 0 )
            {
                structvec[StructureMgr.m_CurrStructIndex()]->GetStructSettingsPtr()->SetExportFileName( newfile, vsp::FEA_IGES_FILE_NAME );
            }
        }
    }
    else if ( device == &m_SelectSTEPFile )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            string newfile = m_ScreenMgr->GetSelectFileScreen()->FileChooser( "Select STEP .stp file.", "*.stp" );
            if ( newfile.compare( "" ) != 0 )
            {
                structvec[StructureMgr.m_CurrStructIndex()]->GetStructSettingsPtr()->SetExportFileName( newfile, vsp::FEA_STEP_FILE_NAME );
            }
        }
    }
    else if ( device == &m_AddFeaBCButton )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            FeaStructure* curr_struct = structvec[StructureMgr.m_CurrStructIndex()];

            if ( curr_struct )
            {
                FeaBC* feabc = curr_struct->AddFeaBC();

                if ( feabc )
                {
                    StructureMgr.SetCurrBCIndex( curr_struct->NumFeaBCs() - 1 );
//                    feabc->Update();
                }
            }
        }
        else
        {
            AddOutputText( "Error: No Structure Selected\n" );
        }
    }
    else if ( device == &m_DelFeaBCButton )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            FeaStructure* curr_struct = structvec[StructureMgr.m_CurrStructIndex()];

            if ( curr_struct )
            {

                if ( curr_struct->ValidFeaBCInd( StructureMgr.GetCurrBCIndex() ) )
                {
                    curr_struct->DelFeaBC( StructureMgr.GetCurrBCIndex() );

                    StructureMgr.SetCurrBCIndex( StructureMgr.GetCurrBCIndex() - 1 );
                }
                else
                {
                    StructureMgr.SetCurrBCIndex( -1 );
                }
            }
        }
    }
    else if ( device == &m_FeaBCPartChoice )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure * > structvec = StructureMgr.GetAllFeaStructs();

            FeaStructure *curr_struct = structvec[ StructureMgr.m_CurrStructIndex() ];

            if ( curr_struct )
            {
                m_SelectedBCPartChoice = m_FeaBCPartChoice.GetVal();

                if ( m_SelectedBCPartChoice >= 0 && m_SelectedBCPartChoice < m_FeaBCPartChoiceIDVec.size() &&
                     m_FeaBCPartChoiceIDVec.size() > 0 )
                {
                    if ( curr_struct->ValidFeaBCInd( StructureMgr.GetCurrBCIndex()))
                    {
                        FeaBC *curr_bc = curr_struct->GetFeaBC( StructureMgr.GetCurrBCIndex());

                        if ( curr_bc->m_FeaBCType() == vsp::FEA_BC_PART )
                        {
                            curr_bc->SetPartID( m_FeaBCPartChoiceIDVec[ m_SelectedBCPartChoice ] );
                        }
                    }
                }
            }
        }
    }
    else if ( device == &m_FeaBCSubSurfChoice )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure * > structvec = StructureMgr.GetAllFeaStructs();

            FeaStructure *curr_struct = structvec[ StructureMgr.m_CurrStructIndex() ];

            if ( curr_struct )
            {
                m_SelectedBCSubSurfChoice = m_FeaBCSubSurfChoice.GetVal();

                if ( m_SelectedBCSubSurfChoice >= 0 && m_SelectedBCSubSurfChoice < m_FeaBCSubSurfChoiceIDVec.size() &&
                     m_FeaBCSubSurfChoiceIDVec.size() > 0 )
                {
                    if ( curr_struct->ValidFeaBCInd( StructureMgr.GetCurrBCIndex()))
                    {
                        FeaBC *curr_bc = curr_struct->GetFeaBC( StructureMgr.GetCurrBCIndex());

                        if ( curr_bc->m_FeaBCType() == vsp::FEA_BC_SUBSURF )
                        {
                            curr_bc->SetSubSurfID( m_FeaBCSubSurfChoiceIDVec[ m_SelectedBCSubSurfChoice ] );
                        }
                    }
                }
            }
        }
    }

    m_ScreenMgr->SetUpdateFlag( true );
}

void StructScreen::OrientStructure( VSPGraphic::Common::VSPenum type )
{
    if ( !StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
    {
        return;
    }

    FeaStructure* curr_struct = StructureMgr.GetAllFeaStructs()[StructureMgr.m_CurrStructIndex()];
    Vehicle* veh = m_ScreenMgr->GetVehiclePtr();

    if ( veh && curr_struct )
    {
        Geom* curr_geom = veh->FindGeom( curr_struct->GetParentGeomID() );

        MainVSPScreen* main = dynamic_cast<MainVSPScreen*>( m_ScreenMgr->GetScreen( m_ScreenMgr->VSP_MAIN_SCREEN ) );

        if ( curr_geom && main )
        {
            if ( type == VSPGraphic::Common::VSP_CAM_FRONT )
            {
                curr_geom->m_GuiDraw.SetDisplayType( DISPLAY_TYPE::DISPLAY_DEGEN_SURF );
            }
            else
            {
                curr_geom->m_GuiDraw.SetDisplayType( DISPLAY_TYPE::DISPLAY_DEGEN_CAMBER );
            }

            curr_geom->m_GuiDraw.SetDrawType( vsp::DRAW_TYPE::GEOM_DRAW_SHADE );
            curr_geom->m_GuiDraw.SetDispSubSurfFlag( true );

            veh->ShowSet( 0 );
            veh->HideAllExcept( curr_struct->GetParentGeomID() );
            veh->ClearActiveGeom();

            VSPGUI::VspGlWindow * glwin = main->GetGLWindow();

            if ( !glwin )
            {
                return;
            }

            glwin->setView( type );

            BndBox bbox = curr_geom->GetBndBox();

            vec3d p = bbox.GetCenter();
            double d = bbox.DiagDist();
            int wid = glwin->pixel_w();
            int ht = glwin->pixel_h();

            float z = d * ( wid < ht ? 1.f / wid : 1.f / ht );

            // Validate pointers
            if ( !glwin->getGraphicEngine() )
            {
                return;
            }
            if ( !glwin->getGraphicEngine()->getDisplay() )
            {
                return;
            }
            if ( !glwin->getGraphicEngine()->getDisplay()->getCamera() )
            {
                return;
            }

            glwin->getGraphicEngine()->getDisplay()->setCOR( (float)-p.x(), (float)-p.y(), (float)-p.z() );
            glwin->getGraphicEngine()->getDisplay()->relativePan( 0.0f, 0.0f );
            glwin->getGraphicEngine()->getDisplay()->getCamera()->relativeZoom( z );

            ManageViewScreen * viewScreen = dynamic_cast<ManageViewScreen*>( m_ScreenMgr->GetScreen( ScreenMgr::VSP_VIEW_SCREEN ) );

            if ( viewScreen )
            {
                viewScreen->UpdateCOR();
                viewScreen->UpdatePan();
                viewScreen->UpdateZoom();
            }
        }
    }
}

void StructScreen::OrientWing()
{
    if ( !StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
    {
        return;
    }

    FeaStructure* curr_struct = StructureMgr.GetAllFeaStructs()[StructureMgr.m_CurrStructIndex()];
    Vehicle* veh = m_ScreenMgr->GetVehiclePtr();

    if ( veh && curr_struct )
    {
        Geom* current_wing = veh->FindGeom( curr_struct->GetParentGeomID() );

        if ( !current_wing )
        {
            return;
        }

        if ( current_wing->GetType().m_Type == MS_WING_GEOM_TYPE )
        {
            BndBox wing_bbox = current_wing->GetBndBox();

            double del_y = wing_bbox.GetMax( 1 ) - wing_bbox.GetMin( 1 );
            double del_z = wing_bbox.GetMax( 2 ) - wing_bbox.GetMin( 2 );

            VSPGraphic::Common::VSPenum type;

            // Select a side view if the wing geom is a vertical stabilizer
            if ( del_z > del_y )
            {
                type = VSPGraphic::Common::VSP_CAM_LEFT;
            }
            else
            {
                type = VSPGraphic::Common::VSP_CAM_TOP;
            }

            OrientStructure( type );
        }
    }
}

void StructScreen::LoadDrawObjs( vector< DrawObj* > &draw_obj_vec )
{
    Vehicle* veh = m_ScreenMgr->GetVehiclePtr();

    if ( !veh )
    {
        return;
    }

    if ( IsShown() )
    {
        if ( !FeaMeshMgr.GetFeaMeshInProgress() && StructureMgr.ValidTotalFeaStructInd( StructureMgr.m_CurrStructIndex() ) )
        {
            vector < FeaStructure* > totalstructvec = StructureMgr.GetAllFeaStructs();

            FeaStructure* curr_struct = totalstructvec[StructureMgr.m_CurrStructIndex()];

            if ( !curr_struct )
            {
                return;
            }

            vector < FeaPart* > partvec = curr_struct->GetFeaPartVec();
            vector < int > active_part_index_vec;

            for ( size_t i = 0; i < m_SelectedPartIndexVec.size(); i++ )
            {
                if ( m_SelectedPartIndexVec[i] < curr_struct->NumFeaParts() )
                {
                    active_part_index_vec.push_back( m_SelectedPartIndexVec[i] );
                }
            }

            curr_struct->HighlightFeaParts( active_part_index_vec );

            for ( unsigned int i = 0; i < (int)partvec.size(); i++ )
            {
                if ( partvec[i]->m_DrawFeaPartFlag() )
                {
                    partvec[i]->LoadDrawObjs( draw_obj_vec );
                }
            }

            vector < SubSurface* > subsurf_vec = curr_struct->GetFeaSubSurfVec();
            vector < int > active_index_vec;

            for ( size_t i = 0; i < m_SelectedPartIndexVec.size(); i++ )
            {
                if ( m_SelectedPartIndexVec[i] >= curr_struct->NumFeaParts() )
                {
                    active_index_vec.push_back( m_SelectedPartIndexVec[i] - curr_struct->NumFeaParts() );
                }
            }

            curr_struct->RecolorFeaSubSurfs( active_index_vec );

            for ( unsigned int i = 0; i < (int)subsurf_vec.size(); i++ )
            {
                if ( subsurf_vec[i]->m_DrawFeaPartFlag() )
                {
                    subsurf_vec[i]->LoadDrawObjs( draw_obj_vec );
                }
            }
        }

        // Load Draw Objects for FeaMesh
        FeaMeshMgr.LoadDrawObjs( draw_obj_vec );
    }
}
