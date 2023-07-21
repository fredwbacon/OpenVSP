//
// This file is released under the terms of the NASA Open Source Agreement (NOSA)
// version 1.3 as detailed in the LICENSE file which accompanies this software.
//

// API.h: interface for the Vehicle Class and Vehicle Mgr Singleton.
// J.R Gloudemans
//
//////////////////////////////////////////////////////////////////////

#if !defined(VSPDEFINES__INCLUDED_)
#define VSPDEFINES__INCLUDED_

namespace vsp
{
enum ABS_REL_FLAG { ABS,
                    REL
                  };

enum AIRFOIL_EXPORT_TYPE { SELIG_AF_EXPORT,
                           BEZIER_AF_EXPORT
                         };

enum ANG_UNITS { ANG_RAD,
                 ANG_DEG
               };

enum ATMOS_TYPE { ATMOS_TYPE_US_STANDARD_1976 = 0,
                  ATMOS_TYPE_HERRINGTON_1966,
                  ATMOS_TYPE_MANUAL_P_R,
                  ATMOS_TYPE_MANUAL_P_T,
                  ATMOS_TYPE_MANUAL_R_T,
                  ATMOS_TYPE_MANUAL_RE_L
                }; // Flowstream Type

enum ATTACH_TRANS_TYPE { ATTACH_TRANS_NONE = 0,
                         ATTACH_TRANS_COMP,
                         ATTACH_TRANS_UV,
                         ATTACH_TRANS_RST,
                         ATTACH_TRANS_LMN,
                         ATTACH_TRANS_NUM_TYPES
};

enum ATTACH_ROT_TYPE { ATTACH_ROT_NONE = 0,
                       ATTACH_ROT_COMP,
                       ATTACH_ROT_UV,
                       ATTACH_ROT_RST,
                       ATTACH_ROT_LMN,
                       ATTACH_ROT_NUM_TYPES
};

enum BOR_MODE { BOR_FLOWTHROUGH,
                BOR_UPPER,
                BOR_LOWER,
                BOR_NUM_MODES
              };

enum CAMBER_INPUT_FLAG { MAX_CAMB,
                         DESIGN_CL
};

enum CAP_TYPE { NO_END_CAP,
                FLAT_END_CAP,
                ROUND_END_CAP,
                EDGE_END_CAP,
                SHARP_END_CAP,
                POINT_END_CAP,
                ROUND_EXT_END_CAP_NONE,
                ROUND_EXT_END_CAP_LE,
                ROUND_EXT_END_CAP_TE,
                ROUND_EXT_END_CAP_BOTH,
                NUM_END_CAP_OPTIONS
              };

enum CFD_CONTROL_TYPE {     CFD_MIN_EDGE_LEN,
                            CFD_MAX_EDGE_LEN,
                            CFD_MAX_GAP,
                            CFD_NUM_CIRCLE_SEGS,
                            CFD_GROWTH_RATIO,
                            CFD_LIMIT_GROWTH_FLAG,
                            CFD_INTERSECT_SUBSURFACE_FLAG,
                            CFD_HALF_MESH_FLAG,
                            CFD_FAR_FIELD_FLAG,
                            CFD_FAR_MAX_EDGE_LEN,
                            CFD_FAR_MAX_GAP,
                            CFD_FAR_NUM_CIRCLE_SEGS,
                            CFD_FAR_SIZE_ABS_FLAG,
                            CFD_FAR_LENGTH,
                            CFD_FAR_WIDTH,
                            CFD_FAR_HEIGHT,
                            CFD_FAR_X_SCALE,
                            CFD_FAR_Y_SCALE,
                            CFD_FAR_Z_SCALE,
                            CFD_FAR_LOC_MAN_FLAG,
                            CFD_FAR_LOC_X,
                            CFD_FAR_LOC_Y,
                            CFD_FAR_LOC_Z,
                            CFD_SRF_XYZ_FLAG,
                      };

enum CFD_MESH_EXPORT_TYPE { CFD_STL_FILE_NAME,
                            CFD_POLY_FILE_NAME,
                            CFD_TRI_FILE_NAME,
                            CFD_OBJ_FILE_NAME,
                            CFD_DAT_FILE_NAME,
                            CFD_KEY_FILE_NAME,
                            CFD_GMSH_FILE_NAME,
                            CFD_TKEY_FILE_NAME,
                            CFD_FACET_FILE_NAME,
                            CFD_VSPGEOM_FILE_NAME,
                            CFD_NUM_FILE_NAMES,
                          };

enum CFD_MESH_SOURCE_TYPE { POINT_SOURCE,
                            LINE_SOURCE,
                            BOX_SOURCE,
                            ULINE_SOURCE,
                            WLINE_SOURCE,
                            NUM_SOURCE_TYPES,
                          };

enum CFD_VIS_TYPE { TAG,
                    REASON
                  };

enum CF_LAM_EQN { CF_LAM_BLASIUS = 0,
                  CF_LAM_BLASIUS_W_HEAT,
                }; // Friction Coefficient Laminar Eqns ENUM

enum CF_TURB_EQN { CF_TURB_EXPLICIT_FIT_SPALDING = 0,
                   CF_TURB_EXPLICIT_FIT_SPALDING_CHI,
                   CF_TURB_EXPLICIT_FIT_SCHOENHERR,
                   DO_NOT_USE_CF_TURB_IMPLICIT_KARMAN,
                   CF_TURB_IMPLICIT_SCHOENHERR,
                   CF_TURB_IMPLICIT_KARMAN_SCHOENHERR,
                   CF_TURB_POWER_LAW_BLASIUS,
                   CF_TURB_POWER_LAW_PRANDTL_LOW_RE,
                   CF_TURB_POWER_LAW_PRANDTL_MEDIUM_RE,
                   CF_TURB_POWER_LAW_PRANDTL_HIGH_RE,
                   CF_TURB_SCHLICHTING_COMPRESSIBLE,
                   DO_NOT_USE_CF_TURB_SCHLICHTING_INCOMPRESSIBLE,
                   DO_NOT_USE_CF_TURB_SCHLICHTING_PRANDTL,
                   DO_NOT_USE_CF_TURB_SCHULTZ_GRUNOW_HIGH_RE,
                   CF_TURB_SCHULTZ_GRUNOW_SCHOENHERR,
                   DO_NOT_USE_CF_TURB_WHITE_CHRISTOPH_COMPRESSIBLE,
                   CF_TURB_ROUGHNESS_SCHLICHTING_AVG,
                   DO_NOT_USE_CF_TURB_ROUGHNESS_SCHLICHTING_LOCAL,
                   DO_NOT_USE_CF_TURB_ROUGHNESS_WHITE,
                   CF_TURB_ROUGHNESS_SCHLICHTING_AVG_FLOW_CORRECTION,
                   CF_TURB_HEATTRANSFER_WHITE_CHRISTOPH
                 }; // Friction Coefficient Turbulent Eqns ENUM

enum CHEVRON_TYPE { CHEVRON_NONE,
                    CHEVRON_PARTIAL,
                    CHEVRON_FULL,
                    CHEVRON_NUM_TYPES
                  };

enum CHEVRON_W01_MODES { CHEVRON_W01_SE, // Start and End
                         CHEVRON_W01_CW, // Center and Width
                         CHEVRON_W01_NUM_MODES
                       };

enum COLLISION_ERRORS { COLLISION_OK,
                        COLLISION_INTERSECT_NO_SOLUTION,
                        COLLISION_CLEAR_NO_SOLUTION,
                      };

enum COMPUTATION_FILE_TYPE  {   NO_FILE_TYPE        = 0,
                                COMP_GEOM_TXT_TYPE  = 1,
                                COMP_GEOM_CSV_TYPE  = 1<<1,
                                DRAG_BUILD_TSV_TYPE_DEPRECATED = 1<<2,
                                SLICE_TXT_TYPE      = 1<<3,
                                MASS_PROP_TXT_TYPE  = 1<<4,
                                DEGEN_GEOM_CSV_TYPE = 1<<5,
                                DEGEN_GEOM_M_TYPE   = 1<<6,
                                CFD_STL_TYPE        = 1<<7,
                                CFD_POLY_TYPE       = 1<<8,
                                CFD_TRI_TYPE        = 1<<9,
                                CFD_OBJ_TYPE        = 1<<10,
                                CFD_DAT_TYPE        = 1<<11,
                                CFD_KEY_TYPE        = 1<<12,
                                CFD_GMSH_TYPE       = 1<<13,
                                CFD_SRF_TYPE_DEPRECATED        = 1<<14,
                                CFD_TKEY_TYPE       = 1<<15,
                                PROJ_AREA_CSV_TYPE  = 1<<16,
                                WAVE_DRAG_TXT_TYPE  = 1<<17,
                                VSPAERO_PANEL_TRI_TYPE = 1<<18,
                                DRAG_BUILD_CSV_TYPE = 1<<19,
                                CFD_FACET_TYPE      = 1<<20,
                                CFD_CURV_TYPE_DEPRECATED       = 1<<21,
                                CFD_PLOT3D_TYPE_DEPRECATED     = 1<<22,
                                CFD_VSPGEOM_TYPE    = 1<<23,
                                VSPAERO_VSPGEOM_TYPE = 1<<24,
                            };

enum DELIM_TYPE { DELIM_COMMA,
                  DELIM_USCORE,
                  DELIM_SPACE,
                  DELIM_NONE,
                  DELIM_NUM_TYPES
                };

enum DIMENSION_SET { SET_3D,
                     SET_2D,
                   };

enum DIR_INDEX { X_DIR = 0,
                 Y_DIR = 1,
                 Z_DIR = 2,
                 ALL_DIR = 3
               };

enum DISPLAY_TYPE { DISPLAY_BEZIER,
                    DISPLAY_DEGEN_SURF,
                    DISPLAY_DEGEN_PLATE,
                    DISPLAY_DEGEN_CAMBER
                  };

enum DRAW_TYPE { GEOM_DRAW_WIRE,
                 GEOM_DRAW_HIDDEN,
                 GEOM_DRAW_SHADE,
                 GEOM_DRAW_TEXTURE,
                 GEOM_DRAW_NONE
               };

enum ENGINE_GEOM_IO_TYPE { ENGINE_GEOM_NONE,
                           ENGINE_GEOM_INLET,
                           ENGINE_GEOM_INLET_OUTLET,
                           ENGINE_GEOM_OUTLET,
                           ENGINE_GEOM_IO_NUM_TYPES
                         };

enum ENGINE_GEOM_TYPE { ENGINE_GEOM_FLOWTHROUGH,
                        ENGINE_GEOM_TO_LIP,
                        ENGINE_GEOM_FLOWPATH,
                        ENGINE_GEOM_TO_FACE,
                        ENGINE_GEOM_NUM_TYPES
                      };

enum ENGINE_LOC_MODE { ENGINE_LOC_INDEX,
                       ENGINE_LOC_U
                     };

enum ENGINE_LOC_INDEX_TYPES { ENGINE_LOC_INLET_LIP,
                              ENGINE_LOC_INLET_FACE,
                              ENGINE_LOC_OUTLET_LIP,
                              ENGINE_LOC_OUTLET_FACE,
                              ENGINE_LOC_NUM
                            };

// For inlet, all types are valid.  However, for outlet, only NATIVE through EXTEND are valid.
// Both inlet and outlet, NATIVE is the default.  By making NATIVE the first entry, the Parm::Init
// can be used to enforce valid values for both inlet and outlet.
enum ENGINE_MODE_TYPE { ENGINE_MODE_FLOWTHROUGH,
                        ENGINE_MODE_FLOWTHROUGH_NEG,
                        ENGINE_MODE_TO_LIP,
                        ENGINE_MODE_TO_FACE,
                        ENGINE_MODE_TO_FACE_NEG,
                        ENGINE_MODE_EXTEND,
                        ENGINE_MODE_NUM_TYPES
                 };

enum ERROR_CODE {   VSP_OK,
                    VSP_INVALID_PTR,
                    VSP_INVALID_TYPE,
                    VSP_CANT_FIND_TYPE,
                    VSP_CANT_FIND_PARM,
                    VSP_CANT_FIND_NAME,
                    VSP_INVALID_GEOM_ID,
                    VSP_FILE_DOES_NOT_EXIST,
                    VSP_FILE_WRITE_FAILURE,
                    VSP_FILE_READ_FAILURE,
                    VSP_WRONG_XSEC_TYPE,
                    VSP_WRONG_FILE_TYPE,
                    VSP_INDEX_OUT_RANGE,
                    VSP_INVALID_XSEC_ID,
                    VSP_INVALID_ID,
                    VSP_CANT_SET_NOT_EQ_PARM,
                    VSP_AMBIGUOUS_SUBSURF,
                    VSP_INVALID_VARPRESET_SETNAME,
                    VSP_INVALID_VARPRESET_GROUPNAME,
                    VSP_CONFORMAL_PARENT_UNSUPPORTED,
                    VSP_UNEXPECTED_RESET_REMAP_ID,
                    VSP_INVALID_INPUT_VAL,
                    VSP_INVALID_CF_EQN,
                    VSP_INVALID_DRIVERS,
                    VSP_ADV_LINK_BUILD_FAIL,
                    VSP_DEPRECATED
                };

enum EXCRES_TYPE { EXCRESCENCE_COUNT = 0,
                   EXCRESCENCE_CD,
                   EXCRESCENCE_PERCENT_GEOM,
                   EXCRESCENCE_MARGIN,
                   EXCRESCENCE_DRAGAREA,
                 }; // Excrescence Type ENUM

enum EXPORT_TYPE {  EXPORT_FELISA,
                    EXPORT_XSEC,
                    EXPORT_STL,
                    EXPORT_AWAVE,
                    EXPORT_NASCART,
                    EXPORT_POVRAY,
                    EXPORT_CART3D,
                    EXPORT_VSPGEOM,
                    EXPORT_VORXSEC,
                    EXPORT_XSECGEOM,
                    EXPORT_GMSH,
                    EXPORT_X3D,
                    EXPORT_STEP,
                    EXPORT_PLOT3D,
                    EXPORT_IGES,
                    EXPORT_BEM,
                    EXPORT_DXF,
                    EXPORT_FACET,
                    EXPORT_SVG,
                    EXPORT_PMARC,
                    EXPORT_OBJ,
                    EXPORT_SELIG_AIRFOIL,
                    EXPORT_BEZIER_AIRFOIL,
                    EXPORT_IGES_STRUCTURE,
                    EXPORT_STEP_STRUCTURE
                 };

enum FEA_BC_TYPE { FEA_BC_STRUCTURE,
                   FEA_BC_PART,
                   FEA_BC_SUBSURF,
                   FEA_NUM_BC_TYPES
                 };

enum FEA_BC_MODE { FEA_BCM_USER,
                   FEA_BCM_ALL,
                   FEA_BCM_PIN,
                   FEA_BCM_SYMM,
                   FEA_BCM_ASYMM,
                   FEA_NUM_BCM_MODES
                  };

enum FEA_CROSS_SECT_TYPE { FEA_XSEC_GENERAL = 0,
                           FEA_XSEC_CIRC,
                           FEA_XSEC_PIPE,
                           FEA_XSEC_I,
                           FEA_XSEC_RECT,
                           FEA_XSEC_BOX
                         };

enum FEA_EXPORT_TYPE { FEA_MASS_FILE_NAME,
                       FEA_NASTRAN_FILE_NAME,
                       FEA_NKEY_FILE_NAME,
                       FEA_CALCULIX_FILE_NAME,
                       FEA_STL_FILE_NAME,
                       FEA_GMSH_FILE_NAME,
                       FEA_SRF_FILE_NAME,
                       FEA_CURV_FILE_NAME,
                       FEA_PLOT3D_FILE_NAME,
                       FEA_IGES_FILE_NAME,
                       FEA_STEP_FILE_NAME,
                       FEA_NUM_FILE_NAMES
                     };

enum FEA_MATERIAL_TYPE { FEA_ISOTROPIC,
                         FEA_ENG_ORTHO,
                         FEA_NUM_MAT_TYPES
                       };

enum FEA_ORIENTATION_TYPE { FEA_ORIENT_GLOBAL_X,
                            FEA_ORIENT_GLOBAL_Y,
                            FEA_ORIENT_GLOBAL_Z,
                            FEA_ORIENT_COMP_X,
                            FEA_ORIENT_COMP_Y,
                            FEA_ORIENT_COMP_Z,
                            FEA_ORIENT_PART_U,
                            FEA_ORIENT_PART_V,
                            FEA_ORIENT_OML_U,
                            FEA_ORIENT_OML_V,
                            FEA_ORIENT_OML_R,
                            FEA_ORIENT_OML_S,
                            FEA_ORIENT_OML_T,
                            FEA_NUM_ORIENT_TYPES
                          };

enum FEA_PART_ELEMENT_TYPE { FEA_DEPRECATED = -1,
                             FEA_SHELL = 0,
                             FEA_BEAM,
                             FEA_SHELL_AND_BEAM,
                             FEA_NO_ELEMENTS,
                             FEA_NUM_ELEMENT_TYPES
                           };

enum FEA_SHELL_TREATMENT_TYPE { FEA_KEEP = 0,
                                FEA_DELETE,
                                FEA_NUM_SHELL_TREATMENT_TYPES
                              };

enum FEA_PART_TYPE { FEA_SLICE = 0,
                     FEA_RIB,
                     FEA_SPAR,
                     FEA_FIX_POINT,
                     FEA_DOME,
                     FEA_RIB_ARRAY,
                     FEA_SLICE_ARRAY,
                     FEA_SKIN,
                     FEA_TRIM,
                     FEA_NUM_TYPES
                   };

enum FEA_SLICE_TYPE { XY_BODY = 0,
                      YZ_BODY,
                      XZ_BODY,
                      XY_ABS,
                      YZ_ABS,
                      XZ_ABS,
                      SPINE_NORMAL
                    };

enum FEA_UNIT_TYPE { SI_UNIT = 0, // m, kg
                     CGS_UNIT, // cm, g
                     MPA_UNIT, // mm, tonne
                     BFT_UNIT, // ft, slug
                     BIN_UNIT // in, lbf*sec^2/in
                   };

enum FEA_RIB_NORMAL { NO_NORMAL,
                      LE_NORMAL,
                      TE_NORMAL,
                      SPAR_NORMAL
                    };

enum FF_B_EQN { FF_B_MANUAL = 0,
                FF_B_SCHEMENSKY_FUSE,
                FF_B_SCHEMENSKY_NACELLE,
                FF_B_HOERNER_STREAMBODY,
                FF_B_TORENBEEK,
                FF_B_SHEVELL,
                FF_B_COVERT,
                FF_B_JENKINSON_FUSE,
                FF_B_JENKINSON_WING_NACELLE,
                FF_B_JENKINSON_AFT_FUSE_NACELLE,
              }; // FF Body Eqn ENUM

enum FF_W_EQN { FF_W_MANUAL = 0,
                FF_W_EDET_CONV,
                FF_W_EDET_ADV,
                FF_W_HOERNER,
                FF_W_COVERT,
                FF_W_SHEVELL,
                FF_W_KROO,
                FF_W_TORENBEEK,
                FF_W_DATCOM,
                FF_W_SCHEMENSKY_6_SERIES_AF,
                FF_W_SCHEMENSKY_4_SERIES_AF,
                FF_W_JENKINSON_WING,
                FF_W_JENKINSON_TAIL,
                FF_W_SCHEMENSKY_SUPERCRITICAL_AF,
              }; // FF Wing Eqn ENUM

enum FREESTREAM_PD_UNITS { PD_UNITS_IMPERIAL = 0,
                           PD_UNITS_METRIC
                         }; // Parasite Drag Freestream Units

enum GENDER { MALE,
              FEMALE
            };

enum GUI_GEOM_SCREEN { POD_GEOM_SCREEN,
                       FUSELAGE_GEOM_SCREEN,
                       MS_WING_GEOM_SCREEN,
                       BLANK_GEOM_SCREEN,
                       MESH_GEOM_SCREEN,
                       STACK_GEOM_SCREEN,
                       CUSTOM_GEOM_SCREEN,
                       PT_CLOUD_GEOM_SCREEN,
                       PROP_GEOM_SCREEN,
                       HINGE_GEOM_SCREEN,
                       MULT_GEOM_SCREEN,
                       CONFORMAL_SCREEN,
                       ELLIPSOID_GEOM_SCREEN,
                       BOR_GEOM_SCREEN,
                       WIRE_FRAME_GEOM_SCREEN,
                       HUMAN_GEOM_SCREEN,
                       NUM_GEOM_SCREENS,
                       ALL_GEOM_SCREENS
                     };

enum GUI_VSP_SCREEN { VSP_ADV_LINK_SCREEN,
                      VSP_AERO_STRUCT_SCREEN,
                      VSP_AIRFOIL_CURVES_EXPORT_SCREEN,
                      VSP_AIRFOIL_POINTS_EXPORT_SCREEN,
                      VSP_BACKGROUND_SCREEN,
                      VSP_BEM_OPTIONS_SCREEN,
                      VSP_CFD_MESH_SCREEN,
                      VSP_CLIPPING_SCREEN,
                      VSP_COMP_GEOM_SCREEN,
                      VSP_COR_SCREEN,
                      VSP_CURVE_EDIT_SCREEN,
                      VSP_DEGEN_GEOM_SCREEN,
                      VSP_DESIGN_VAR_SCREEN,
                      VSP_DXF_OPTIONS_SCREEN,
                      VSP_EXPORT_SCREEN,
                      VSP_FEA_PART_EDIT_SCREEN,
                      VSP_FEA_XSEC_SCREEN,
                      VSP_FIT_MODEL_SCREEN,
                      VSP_IGES_OPTIONS_SCREEN,
                      VSP_IGES_STRUCTURE_OPTIONS_SCREEN,
                      VSP_EXPORT_CUSTOM_SCRIPT,
                      VSP_IMPORT_SCREEN,
                      VSP_LIGHTING_SCREEN,
                      VSP_MANAGE_GEOM_SCREEN,
                      VSP_MANAGE_TEXTURE_SCREEN,
                      VSP_MASS_PROP_SCREEN,
                      VSP_MATERIAL_EDIT_SCREEN,
                      VSP_MEASURE_SCREEN,
                      VSP_SNAP_TO_SCREEN,
                      VSP_PARASITE_DRAG_SCREEN,
                      VSP_PARM_DEBUG_SCREEN,
                      VSP_PARM_LINK_SCREEN,
                      VSP_PARM_SCREEN,
                      VSP_PICK_SET_SCREEN,
                      VSP_PROJECTION_SCREEN,
                      VSP_PSLICE_SCREEN,
                      VSP_SCREENSHOT_SCREEN,
                      VSP_SELECT_FILE_SCREEN,
                      VSP_SET_EDITOR_SCREEN,
                      VSP_STEP_OPTIONS_SCREEN,
                      VSP_STEP_STRUCTURE_OPTIONS_SCREEN,
                      VSP_STL_OPTIONS_SCREEN,
                      VSP_STRUCT_SCREEN,
                      VSP_STRUCT_ASSEMBLY_SCREEN,
                      VSP_SURFACE_INTERSECTION_SCREEN,
                      VSP_SVG_OPTIONS_SCREEN,
                      VSP_USER_PARM_SCREEN,
                      VSP_VAR_PRESET_SCREEN,
                      VSP_VIEW_SCREEN,
                      VSP_VSPAERO_PLOT_SCREEN,
                      VSP_VSPAERO_SCREEN,
                      VSP_XSEC_SCREEN,
                      VSP_WAVEDRAG_SCREEN,
                      VSP_MAIN_SCREEN,   /* Leave at end of list, helps draw after update. */
                      VSP_NUM_SCREENS
                     };

enum INIT_EDIT_XSEC_TYPE { EDIT_XSEC_CIRCLE,
                           EDIT_XSEC_ELLIPSE,
                           EDIT_XSEC_RECTANGLE
                         };

enum IMPORT_TYPE {  IMPORT_STL,
                    IMPORT_NASCART,
                    IMPORT_CART3D_TRI,
                    IMPORT_XSEC_MESH,
                    IMPORT_PTS,
                    IMPORT_V2,
                    IMPORT_BEM,
                    IMPORT_XSEC_WIRE,
                    IMPORT_P3D_WIRE
                 };

enum INTERSECT_EXPORT_TYPE { INTERSECT_SRF_FILE_NAME,
                             INTERSECT_CURV_FILE_NAME,
                             INTERSECT_PLOT3D_FILE_NAME,
                             INTERSECT_IGES_FILE_NAME,
                             INTERSECT_STEP_FILE_NAME,
                             INTERSECT_NUM_FILE_NAMES
                           };

enum LEN_UNITS { LEN_MM,
                 LEN_CM,
                 LEN_M,
                 LEN_IN,
                 LEN_FT,
                 LEN_YD,
                 LEN_UNITLESS,
                 NUM_LEN_UNIT
               };

enum MASS_UNIT { MASS_UNIT_G = 0,
                 MASS_UNIT_KG,
                 MASS_UNIT_TONNE,
                 MASS_UNIT_LBM,
                 MASS_UNIT_SLUG,
                 MASS_LBFSEC2IN, // lbf*sec^2/in
                 NUM_MASS_UNIT
               }; // Mass Units ENUM

// Order is important.
enum MESH_REASON { NO_REASON,
                   MAX_LEN_CONSTRAINT,
                   CURV_GAP,
                   CURV_NCIRCSEG,
                   SOURCES,
                   MIN_LEN_CONSTRAINT,               // MAX_LEN_CONSTRAINT + MIN_LEN_INCREMENT
                   MIN_LEN_CONSTRAINT_CURV_GAP,      // CURV_GAP + MIN_LEN_INCREMENT
                   MIN_LEN_CONSTRAINT_CURV_NCIRCSEG, // CURV_NCIRCSEG + MIN_LEN_INCREMENT
                   MIN_LEN_CONSTRAINT_SOURCES,       // Placeholder -- min len not applied to sources.
                   GROW_LIMIT_MAX_LEN_CONSTRAINT,
                   GROW_LIMIT_CURV_GAP,
                   GROW_LIMIT_CURV_NCIRCSEG,
                   GROW_LIMIT_SOURCES,
                   GROW_LIMIT_MIN_LEN_CONSTRAINT,
                   GROW_LIMIT_MIN_LEN_CONSTRAINT_CURV_GAP,
                   GROW_LIMIT_MIN_LEN_CONSTRAINT_CURV_NCIRCSEG,
                   GROW_LIMIT_MIN_LEN_CONSTRAINT_SOURCES,
                   NUM_MESH_REASON,
                   MIN_LEN_INCREMENT = MIN_LEN_CONSTRAINT - MAX_LEN_CONSTRAINT,
                   GROW_LIMIT_INCREMENT = GROW_LIMIT_CURV_GAP - CURV_GAP,
                   MIN_GROW_LIMIT = GROW_LIMIT_CURV_GAP
                 };

enum PARM_TYPE { PARM_DOUBLE_TYPE,
                 PARM_INT_TYPE,
                 PARM_BOOL_TYPE,
                 PARM_FRACTION_TYPE,
                 PARM_LIMITED_INT_TYPE,
                 PARM_NOTEQ_TYPE,
                 PARM_POWER_INT_TYPE
               };

enum PATCH_TYPE { PATCH_NONE,
                  PATCH_POINT,
                  PATCH_LINE,
                  PATCH_COPY,
                  PATCH_HALFWAY,
                  PATCH_NUM_TYPES
                };

enum PCURV_TYPE { LINEAR,
                  PCHIP,
                  CEDIT,
                  APPROX_CEDIT,
                  NUM_PCURV_TYPE
                };

enum PRES_UNITS { PRES_UNIT_PSF = 0,
                  PRES_UNIT_PSI,
                  PRES_UNIT_BA,
                  PRES_UNIT_PA,
                  PRES_UNIT_KPA,
                  PRES_UNIT_MPA,
                  PRES_UNIT_INCHHG,
                  PRES_UNIT_MMHG,
                  PRES_UNIT_MMH20,
                  PRES_UNIT_MB,
                  PRES_UNIT_ATM,
                  NUM_PRES_UNIT
                }; // Pres Units ENUM

enum PROJ_BNDY_TYPE { NO_BOUNDARY,
                      SET_BOUNDARY,
                      GEOM_BOUNDARY,
                      NUM_PROJ_BNDY_OPTIONS
                    };

enum PROJ_DIR_TYPE { X_PROJ,
                     Y_PROJ,
                     Z_PROJ,
                     GEOM_PROJ,
                     VEC_PROJ,
                     NUM_PROJ_DIR_OPTIONS
                   };

enum PROJ_TGT_TYPE { SET_TARGET,
                     GEOM_TARGET,
                     NUM_PROJ_TGT_OPTIONS
                   };

enum PROP_MODE { PROP_BLADES,
                 PROP_BOTH,
                 PROP_DISK
               };

enum PROP_PCURVE { PROP_CHORD,
                   PROP_TWIST,
                   PROP_RAKE,
                   PROP_SKEW,
                   PROP_SWEEP,
                   PROP_THICK,
                   PROP_CLI,
                   PROP_AXIAL,
                   PROP_TANGENTIAL,
                   NUM_PROP_PCURVE
                 };

// Aerodynamic reference area and length
enum REF_WING_TYPE { MANUAL_REF = 0,     // manually specify the reference areas and lengths
                     COMPONENT_REF,      // use a particular wing to calculate the reference area and lengths
                     NUM_REF_TYPES
                   };

enum RES_DATA_TYPE {    INVALID_TYPE = -1,
                        INT_DATA = 0,
                        DOUBLE_DATA = 1,
                        STRING_DATA = 2,
                        VEC3D_DATA = 3,
                        DOUBLE_MATRIX_DATA = 4,
                   };

enum RES_GEOM_TYPE {    MESH_INDEXED_TRI,
                        MESH_SLICE_TRI,
                        GEOM_XSECS,
                        MESH_INDEX_AND_SLICE_TRI,
                   };

enum RHO_UNITS { RHO_UNIT_SLUG_FT3 = 0,
                 RHO_UNIT_G_CM3,
                 RHO_UNIT_KG_M3,
                 RHO_UNIT_TONNE_MM3,
                 RHO_UNIT_LBM_FT3,
                 RHO_UNIT_LBFSEC2_IN4,
                 RHO_UNIT_LBM_IN3,
                 NUM_RHO_UNIT
               }; // Rho Units ENUM

enum SET_TYPE { SET_NONE = -1,
                SET_ALL = 0,
                SET_SHOWN = 1,
                SET_NOT_SHOWN = 2,
                SET_FIRST_USER = 3
              };

enum STEP_REPRESENTATION { STEP_SHELL,
                           STEP_BREP
                         };

enum SUBSURF_INCLUDE { SS_INC_TREAT_AS_PARENT,
                       SS_INC_SEPARATE_TREATMENT,
                       SS_INC_ZERO_DRAG,
                     };

enum SUBSURF_INOUT { INSIDE,
                     OUTSIDE,
                     NONE
                   };

enum SUBSURF_LINE_TYPE { CONST_U,
                         CONST_W
                       };

enum SUBSURF_TYPE { SS_LINE,
                    SS_RECTANGLE,
                    SS_ELLIPSE,
                    SS_CONTROL,
                    SS_LINE_ARRAY,
                    SS_FINITE_LINE,
                    SS_NUM_TYPES
                  };

enum SYM_FLAG {  SYM_XY = ( 1 << 0 ),
                 SYM_XZ = ( 1 << 1 ),
                 SYM_YZ = ( 1 << 2 ),
                 SYM_ROT_X = ( 1 << 3 ),
                 SYM_ROT_Y = ( 1 << 4 ),
                 SYM_ROT_Z = ( 1 << 5 ),
                 SYM_PLANAR_TYPES = 3,
                 SYM_NUM_TYPES = 6
              }; // Symmetry Flags

enum SYM_XSEC_TYP { SYM_NONE,
                    SYM_RL, // Right/Left Symmetry
                    SYM_TB,  // Top/Bottom Symmetry
                    SYM_ALL
                  };

enum TEMP_UNITS { TEMP_UNIT_K = 0,
                  TEMP_UNIT_C,
                  TEMP_UNIT_F,
                  TEMP_UNIT_R,
                  NUM_TEMP_UNIT
                }; // Temp Units ENUM

enum VEL_UNITS { V_UNIT_FT_S = 0,
                 V_UNIT_M_S,
                 V_UNIT_MPH,
                 V_UNIT_KM_HR,
                 V_UNIT_KEAS,
                 V_UNIT_KTAS,
                 V_UNIT_MACH,
               }; // Velocity Units ENUM

enum VIEW_NUM { VIEW_1,
                VIEW_2HOR,
                VIEW_2VER,
                VIEW_4,
              };

enum VIEW_ROT { ROT_0,
                ROT_90,
                ROT_180,
                ROT_270,
              };

enum VIEW_TYPE { VIEW_LEFT,
                 VIEW_RIGHT,
                 VIEW_TOP,
                 VIEW_BOTTOM,
                 VIEW_FRONT,
                 VIEW_REAR,
                 VIEW_NONE,
                 VIEW_NUM_TYPES
               };

enum VSPAERO_ANALYSIS_METHOD { VORTEX_LATTICE,
                               PANEL
                             };

enum VSPAERO_NOISE_TYPE { NOISE_FLYBY,
                          NOISE_FOOTPRINT,
                          NOISE_STEADY,
                        };

enum VSPAERO_NOISE_UNIT { NOISE_SI,
                          NOISE_ENGLISH
                        };

enum VSPAERO_PRECONDITION { PRECON_MATRIX = 0,
                            PRECON_JACOBI,
                            PRECON_SSOR,
                          };

// Values need to match VSPAERO StabControlRun_
enum VSPAERO_STABILITY_TYPE { STABILITY_OFF = 0,
                              STABILITY_DEFAULT = 1,
                              STABILITY_P_ANALYSIS = 2,
                              STABILITY_Q_ANALYSIS = 3,
                              STABILITY_R_ANALYSIS = 4,
                              STABILITY_PITCH = 5,
                              STABILITY_NUM_TYPES = 6
                            };

enum VSPAERO_CLMAX_TYPE { CLMAX_OFF,
                          CLMAX_2D,
                          CLMAX_CARLSON // Carlson's Pressure Correlation
                        };

enum VSP_SURF_CFD_TYPE { CFD_NORMAL,
                         CFD_NEGATIVE,
                         CFD_TRANSPARENT,
                         CFD_STRUCTURE,
                         CFD_STIFFENER,
                         CFD_MEASURE_DUCT,
                         CFD_NUM_TYPES,
                       };

enum VSP_SURF_TYPE { NORMAL_SURF,
                     WING_SURF,
                     DISK_SURF,
                     NUM_SURF_TYPES,
                   };

enum W_HINT { W_RIGHT_0,
              W_BOTTOM,
              W_LEFT,
              W_TOP,
              W_RIGHT_1,
              W_FREE,
            };

enum WING_BLEND { BLEND_FREE,
                  BLEND_ANGLES,
                  BLEND_MATCH_IN_LE_TRAP,
                  BLEND_MATCH_IN_TE_TRAP,
                  BLEND_MATCH_OUT_LE_TRAP,
                  BLEND_MATCH_OUT_TE_TRAP,
                  BLEND_MATCH_IN_ANGLES,
                  BLEND_MATCH_LE_ANGLES,
                  BLEND_NUM_TYPES
                };

enum WING_DRIVERS { AR_WSECT_DRIVER,
                    SPAN_WSECT_DRIVER,
                    AREA_WSECT_DRIVER,
                    TAPER_WSECT_DRIVER,
                    AVEC_WSECT_DRIVER,
                    ROOTC_WSECT_DRIVER,
                    TIPC_WSECT_DRIVER,
                    SECSWEEP_WSECT_DRIVER,
                    NUM_WSECT_DRIVER,
                    SWEEP_WSECT_DRIVER = SECSWEEP_WSECT_DRIVER + 1,
                    SWEEPLOC_WSECT_DRIVER = SECSWEEP_WSECT_DRIVER + 2,
                    SECSWEEPLOC_WSECT_DRIVER = SECSWEEP_WSECT_DRIVER + 3,
                  };

enum XDDM_QUANTITY_TYPE { XDDM_VAR,
                          XDDM_CONST,
                        };

enum XSEC_CLOSE_TYPE { CLOSE_NONE,
                       CLOSE_SKEWLOW,
                       CLOSE_SKEWUP,
                       CLOSE_SKEWBOTH,
                       CLOSE_EXTRAP,
                       CLOSE_NUM_TYPES
                     };

enum XSEC_CRV_TYPE {XS_UNDEFINED = -1,
                    XS_POINT,
                    XS_CIRCLE,
                    XS_ELLIPSE,
                    XS_SUPER_ELLIPSE,
                    XS_ROUNDED_RECTANGLE,
                    XS_GENERAL_FUSE,
                    XS_FILE_FUSE,
                    XS_FOUR_SERIES,
                    XS_SIX_SERIES,
                    XS_BICONVEX,
                    XS_WEDGE,
                    XS_EDIT_CURVE,
                    XS_FILE_AIRFOIL,
                    XS_CST_AIRFOIL,
                    XS_VKT_AIRFOIL,
                    XS_FOUR_DIGIT_MOD,
                    XS_FIVE_DIGIT,
                    XS_FIVE_DIGIT_MOD,
                    XS_ONE_SIX_SERIES,
                    XS_NUM_TYPES
                   };

enum XSEC_DRIVERS { WIDTH_XSEC_DRIVER,     // First two are used for Circle.  Others are used for general XSecCurves
                    AREA_XSEC_DRIVER,      // Area must be second entry.
                    HEIGHT_XSEC_DRIVER,
                    HWRATIO_XSEC_DRIVER,
                    NUM_XSEC_DRIVER,
                    CIRCLE_NUM_XSEC_DRIVER = 2
                  };

enum XSEC_SIDES_TYPE {  XSEC_BOTH_SIDES,
                        XSEC_LEFT_SIDE,
                        XSEC_RIGHT_SIDE
                     };

enum XSEC_TRIM_TYPE { TRIM_NONE,
                      TRIM_X,
                      TRIM_THICK,
                      TRIM_NUM_TYPES
                    };

enum XSEC_TYPE { XSEC_FUSE,
                 XSEC_STACK,
                 XSEC_WING,
                 XSEC_CUSTOM,
                 XSEC_PROP,
                 XSEC_NUM_TYPES
               };

enum XSEC_WIDTH_SHIFT { XS_SHIFT_LE = 0,
                        XS_SHIFT_MID = 1,
                        XS_SHIFT_TE = 2
                      };
}   // Namespace

#endif // !defined(VSPDEFINES__INCLUDED_)
