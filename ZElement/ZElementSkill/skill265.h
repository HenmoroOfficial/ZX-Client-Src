#ifndef __CPPGEN_GNET_SKILL265
#define __CPPGEN_GNET_SKILL265
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill265:public Skill
    {
      public:
        enum
        { SKILL_ID = 265 };
          Skill265 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill265Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 2400;
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
                return 600;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.02 * skill->GetLevel () + 0.01 * skill->GetPlayer ()->GetLevel () / 7);
                skill->SetPlus (285 + 58 * skill->GetLevel () - 0.057 * skill->GetLevel () * skill->GetLevel () +
                                skill->GetLevel () * skill->GetPlayer ()->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill265Stub ():SkillStub (265)
        {
            occupation = 130;
            name = L"ÁÒÑæ³àÓğ";
            nativename = "ÁÒÑæ³àÓğ";
            icon = "ÁÒÑæ¾ªÓğ.dds";
            maxlevel = 8;
            maxlearn = 8;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
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
            action[0] = "¹íÍõ×Ú_ÁÒÑæ¾ªÓğ";
            action[1] = "¹íÍõ×Ú_ÁÒÑæ¾ªÓğ";
            action[2] = "¹íÍõ×Ú_ÁÒÑæ¾ªÓğ";
            action[3] = "¹íÍõ×Ú_ÁÒÑæ¾ªÓğ";
            action[4] = "¹íÍõ×Ú_ÁÒÑæ¾ªÓğ";
            action[5] = "0";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÁÒÑæ¾ªÓğ";
            action[16] = "¹íÍõ×Ú_Æï³Ë_ÁÒÑæ¾ªÓğ";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÁÒÑæ¾ªÓğ";
            action[18] = "¹íÍõ×Ú_Æï³Ë_ÁÒÑæ¾ªÓğ";
            action[19] = "¹íÍõ×Ú_Æï³Ë_ÁÒÑæ¾ªÓğ";
            action[20] = "0";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "¼¼ÄÜ¹¥»÷1";
            rangetype = 0;
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
        virtual ~ Skill265Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0.74 * skill->GetLevel () * skill->GetLevel () + 2.2 * skill->GetLevel () + 100);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 3000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (5);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (8);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (2.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (0.74 * skill->GetLevel () * skill->GetLevel () + 2.2 * skill->GetLevel () + 100),
                               2 * skill->GetLevel (),
                               285 + 58 * skill->GetLevel () - 0.057 * skill->GetLevel () * skill->GetLevel (), skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetLevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetValue (10 + skill->GetLevel ());
            skill->GetVictim ()->SetRepel (1);
            skill->GetVictim ()->SetProbability (1.0 * (15));
            skill->GetVictim ()->SetTime (6000 + 1000);
            skill->GetVictim ()->SetRatio (0.05);
            skill->GetVictim ()->SetBuffid (3);
            skill->GetVictim ()->SetTiansha (1);
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
