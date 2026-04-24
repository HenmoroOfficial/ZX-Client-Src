#ifndef __CPPGEN_GNET_SKILL2033
#define __CPPGEN_GNET_SKILL2033
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2033:public Skill
    {
      public:
        enum
        { SKILL_ID = 2033 };
          Skill2033 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2033Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2033Stub ():SkillStub (2033)
        {
            occupation = 129;
            name = L"·ÉÁúÔÚÌì";
            nativename = "·ÉÁúÔÚÌì";
            icon = "»ÙÌìÃğµØ.dds";
            maxlevel = 1;
            maxlearn = 10;
            type = 3;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = -2;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 0;
            action[0] = "¹íÍõ×Ú_»ÙÌìÃğµØ";
            action[1] = "¹íÍõ×Ú_»ÙÌìÃğµØ";
            action[2] = "¹íÍõ×Ú_»ÙÌìÃğµØ";
            action[3] = "¹íÍõ×Ú_»ÙÌìÃğµØ";
            action[4] = "¹íÍõ×Ú_»ÙÌìÃğµØ";
            action[5] = "¹íÍõ×Ú_»ÙÌìÃğµØ";
            action[6] = "0";
            action[7] = "ÊŞÉñ_Áú¹â";
            action[8] = "ºüÑı_°²ÈçÉ½";
            action[9] = "ĞùÔ¯_°µÄÜ»·";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "ÁéÙí_ÃÍ»÷";
            action[14] = "Ó¢ÕĞ_ÃÍ»÷";
            action[15] = "¹íÍõ×Ú_Æï³Ë_»ÙÌìÃğµØ";
            action[16] = "¹íÍõ×Ú_Æï³Ë_»ÙÌìÃğµØ";
            action[17] = "¹íÍõ×Ú_Æï³Ë_»ÙÌìÃğµØ";
            action[18] = "¹íÍõ×Ú_Æï³Ë_»ÙÌìÃğµØ";
            action[19] = "¹íÍõ×Ú_Æï³Ë_»ÙÌìÃğµØ";
            action[20] = "¹íÍõ×Ú_Æï³Ë_»ÙÌìÃğµØ";
            action[21] = "0";
            action[22] = "ÊŞÉñ_Æï³Ë_Áú¹â";
            action[23] = "ºüÑı_Æï³Ë_°²ÈçÉ½";
            action[24] = "ĞùÔ¯_Æï³Ë_°µÄÜ»·";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "ÁéÙí_Æï³Ë_ÃÍ»÷";
            action[29] = "Ó¢ÕĞ_Æï³Ë_ÃÍ»÷";
            action[30] = "0";
            rangetype = 2;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2033Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 15 * 60000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (25);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (20);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 1000);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format);
        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 *
                                                 ((skill->GetPlayer ()->GetType () == 0 ? 1 : 0) * (skill->GetPlayer ()->GetGender () ==
                                                                                                    1 ? 1 : 0) * 600));
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetDizzy (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 ((skill->GetPlayer ()->GetType () == 0 ? 1 : 0) * (skill->GetPlayer ()->GetGender () ==
                                                                                                    2 ? 1 : 0) * 600));
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetSilent (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 ((skill->GetPlayer ()->GetType () == 0 ? 1 : 0) * (skill->GetPlayer ()->GetGender () ==
                                                                                                    2 ? 1 : 0) * 600));
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetWrap (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetType () != 0 ? 120 : 0));
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->
                SetAmount ((skill->GetPlayer ()->GetHp () <
                            skill->GetPlayer ()->GetVar1 () * 15) ? (skill->GetPlayer ()->GetHp () + 1) : (skill->GetPlayer ()->GetVar1 () * 15));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20);
        }
#endif
    };
}
#endif
