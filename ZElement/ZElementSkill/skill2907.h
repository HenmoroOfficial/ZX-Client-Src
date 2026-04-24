#ifndef __CPPGEN_GNET_SKILL2907
#define __CPPGEN_GNET_SKILL2907
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2907:public Skill
    {
      public:
        enum
        { SKILL_ID = 2907 };
          Skill2907 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2907Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 600;
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2907Stub ():SkillStub (2907)
        {
            occupation = 151;
            name = L"ÔÚ½ÙÄÑÌÓ";
            nativename = "ÔÚ½ÙÄÑÌÓ";
            icon = "ÔÚ½ÙÄÑÌÓ.dds";
            maxlevel = 10;
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
            action[0] = "¹íÍõ×Ú_ÃÍ»÷";
            action[1] = "ÇàÔÆÃÅ_ÃÍ»÷";
            action[2] = "¹íÍõ×Ú_ÃÍ»÷";
            action[3] = "ºÏ»¶ÅÉ_ÃÍ»÷";
            action[4] = "ÌìÒôËÂ_ÃÍ»÷";
            action[5] = "¹íµÀ_ÃÍ»÷";
            action[6] = "0";
            action[7] = "ÊÞÉñ_ÃÍ»÷";
            action[8] = "ºüÑý_ÃÍ»÷";
            action[9] = "0";
            action[10] = "»³¹â_ÃÍ»÷";
            action[11] = "0";
            action[12] = "Ìì»ª_ÃÍ»÷";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÃÍ»÷";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_ÃÍ»÷";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÃÍ»÷";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_ÃÍ»÷";
            action[19] = "ÌìÒôËÂ_Æï³Ë_ÃÍ»÷";
            action[20] = "¹íµÀ_Æï³Ë_ÃÍ»÷";
            action[21] = "0";
            action[22] = "ÊÞÉñ_Æï³Ë_ÃÍ»÷";
            action[23] = "ºüÑý_Æï³Ë_ÃÍ»÷";
            action[24] = "0";
            action[25] = "»³¹â_Æï³Ë_ÃÍ»÷";
            action[26] = "0";
            action[27] = "Ìì»ª_Æï³Ë_ÃÍ»÷";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = true;
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
        virtual ~ Skill2907Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 9);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (2000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 300000;
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 8);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange () + 7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               ((skill->GetLevel () == 1 ? 0.01 : 0) + (skill->GetLevel () == 2 ? 0.03 : 0) + (skill->GetLevel () ==
                                                                                                               3 ? 0.05 : 0) + (skill->GetLevel () ==
                                                                                                                                4 ? 0.08 : 0) +
                                (skill->GetLevel () == 5 ? 0.1 : 0) + (skill->GetLevel () == 6 ? 0.12 : 0) + (skill->GetLevel () ==
                                                                                                              7 ? 0.14 : 0) + (skill->GetLevel () ==
                                                                                                                               8 ? 0.16 : 0) +
                                (skill->GetLevel () == 9 ? 0.2 : 0) + (skill->GetLevel () == 10 ? 0.25 : 0)) * 100);

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel (), 2000);

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (30000);
            skill->GetVictim ()->SetRatio ((skill->GetLevel () == 1 ? 0.01001 : 0) + (skill->GetLevel () == 2 ? 0.03001 : 0) +
                                           (skill->GetLevel () == 3 ? 0.05001 : 0) + (skill->GetLevel () == 4 ? 0.08001 : 0) + (skill->GetLevel () ==
                                                                                                                                5 ? 0.10001 : 0) +
                                           (skill->GetLevel () == 6 ? 0.12001 : 0) + (skill->GetLevel () == 7 ? 0.14001 : 0) + (skill->GetLevel () ==
                                                                                                                                8 ? 0.16001 : 0) +
                                           (skill->GetLevel () == 9 ? 0.20001 : 0) + (skill->GetLevel () == 10 ? 0.25001 : 0));
            skill->GetVictim ()->SetBuffid (9);
            skill->GetVictim ()->SetIncfatalratio (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (300000);
            skill->GetVictim ()->SetRatio (2908);
            skill->GetVictim ()->SetAmount (2908);
            skill->GetVictim ()->SetValue (2908);
            skill->GetVictim ()->SetSetcooldown (1);
            return true;
        }
#endif
    };
}
#endif
