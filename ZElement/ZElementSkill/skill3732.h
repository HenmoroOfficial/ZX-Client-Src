#ifndef __CPPGEN_GNET_SKILL3732
#define __CPPGEN_GNET_SKILL3732
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3732:public Skill
    {
      public:
        enum
        { SKILL_ID = 3732 };
          Skill3732 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3732Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 1600;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 400;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1.1 + 0.04 * 6);
                skill->GetPlayer ()->SetVar1 (INT (skill->GetPlayer ()->GetHasbuff (4199) == 1 ? 1 : 0) * skill->GetPlayer ()->GetRes2 () +
                                              INT (skill->GetPlayer ()->GetHasbuff (4200) ==
                                                   1 ? 1 : 0) * skill->GetPlayer ()->GetRes3 () + INT (skill->GetPlayer ()->GetHasbuff (4201) ==
                                                                                                       1 ? 1 : 0) * skill->GetPlayer ()->GetRes4 () +
                                              INT (skill->GetPlayer ()->GetHasbuff (4202) == 1 ? 1 : 0) * skill->GetPlayer ()->GetRes1 ());
                skill->GetPlayer ()->SetVar2 (INT (skill->GetPlayer ()->GetHasbuff (4199) == 1 ? 1 : 0) +
                                              INT (skill->GetPlayer ()->GetHasbuff (4200) ==
                                                   1 ? 1 : 0) + INT (skill->GetPlayer ()->GetHasbuff (4201) ==
                                                                     1 ? 1 : 0) + INT (skill->GetPlayer ()->GetHasbuff (4202) == 1 ? 1 : 0));
                skill->GetPlayer ()->SetVar3 (INT (skill->GetPlayer ()->GetHasbuff (4199) == 1 ? 1 : 0) * 1 +
                                              INT (skill->GetPlayer ()->GetHasbuff (4200) ==
                                                   1 ? 1 : 0) * 2 + INT (skill->GetPlayer ()->GetHasbuff (4201) ==
                                                                         1 ? 1 : 0) * 3 + INT (skill->GetPlayer ()->GetHasbuff (4202) ==
                                                                                               1 ? 1 : 0) * 4);
                skill->SetPlus (skill->GetPlayer ()->GetVar2 () * skill->GetPlayer ()->GetVar1 () * 6 * (1 + 0.05 * 3));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3732Stub ():SkillStub (3732)
        {
            occupation = 128;
            name = L"Ì«Ô¨Òµº£";
            nativename = "Ì«Ô¨Òµº£";
            icon = "Ì«Ô¨Òµº£.dds";
            maxlevel = 1;
            maxlearn = 1;
            type = 1;
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
            talent[0] = 3405;
            talent[1] = 3406;
            talent_size = 2;
            action[0] = "·ÙÏã¹È_ÍòÏó¾ãËð";
            action[1] = "·ÙÏã¹È_ÍòÏó¾ãËð";
            action[2] = "·ÙÏã¹È_ÍòÏó¾ãËð";
            action[3] = "·ÙÏã¹È_ÍòÏó¾ãËð";
            action[4] = "·ÙÏã¹È_ÍòÏó¾ãËð";
            action[5] = "·ÙÏã¹È_ÍòÏó¾ãËð";
            action[6] = "·ÙÏã¹È_ÍòÏó¾ãËð";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_ÍòÏó¾ãËð";
            action[16] = "·ÙÏã¹È_Æï³Ë_ÍòÏó¾ãËð";
            action[17] = "·ÙÏã¹È_Æï³Ë_ÍòÏó¾ãËð";
            action[18] = "·ÙÏã¹È_Æï³Ë_ÍòÏó¾ãËð";
            action[19] = "·ÙÏã¹È_Æï³Ë_ÍòÏó¾ãËð";
            action[20] = "·ÙÏã¹È_Æï³Ë_ÍòÏó¾ãËð";
            action[21] = "·ÙÏã¹È_Æï³Ë_ÍòÏó¾ãËð";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 3;
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
        virtual ~ Skill3732Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (500);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 35000;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (15);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (14);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 500, 40, 15, (10 + 4 * 6), 0.6 * 6, (20 + 6 * 6), 16, 6);

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
            skill->GetVictim ()->SetProbability (1.0 * ((20 + 6 * 6) * skill->GetPlayer ()->GetVar2 ()));
            skill->GetVictim ()->SetTime (16000);
            skill->GetVictim ()->SetRatio (6 * 10 + skill->GetPlayer ()->GetVar3 ());
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetValue (3613);
            skill->GetVictim ()->SetDelaycast (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (40);
        }
#endif
    };
}
#endif
