#ifndef __CPPGEN_GNET_SKILL264
#define __CPPGEN_GNET_SKILL264
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill264:public Skill
    {
      public:
        enum
        { SKILL_ID = 264 };
          Skill264 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill264Stub:public SkillStub
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
                skill->SetCrit (0.02 * skill->GetT0 ());
                skill->SetRatio (1 + 0.05 * skill->GetT3 ());
                skill->SetPlus (((1.6 + 0.2 * skill->GetLevel ()) * (28 * skill->GetLevel () + 400)) * (1 + 0.08 * skill->GetT2 ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill264Stub ():SkillStub (264)
        {
            occupation = 3;
            name = L"ÐÞÂÞÕ¶";
            nativename = "ÐÞÂÞÕ¶";
            icon = "ÐÞÂÞÕ¶.dds";
            maxlevel = 20;
            maxlearn = 9;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 0;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 0;
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
            preskillnum = 1;
            talent[0] = 620;
            talent[1] = 629;
            talent[2] = 630;
            talent[3] = 634;
            talent_size = 4;
            action[0] = "¹íÍõ×Ú_ÐÞÂÞÕ¶";
            action[1] = "¹íÍõ×Ú_ÐÞÂÞÕ¶";
            action[2] = "¹íÍõ×Ú_ÐÞÂÞÕ¶";
            action[3] = "¹íÍõ×Ú_ÐÞÂÞÕ¶";
            action[4] = "¹íÍõ×Ú_ÐÞÂÞÕ¶";
            action[5] = "¹íÍõ×Ú_ÐÞÂÞÕ¶";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_ÐÞÂÞÕ¶";
            action[16] = "¹íÍõ×Ú_Æï³Ë_ÐÞÂÞÕ¶";
            action[17] = "¹íÍõ×Ú_Æï³Ë_ÐÞÂÞÕ¶";
            action[18] = "¹íÍõ×Ú_Æï³Ë_ÐÞÂÞÕ¶";
            action[19] = "¹íÍõ×Ú_Æï³Ë_ÐÞÂÞÕ¶";
            action[20] = "¹íÍõ×Ú_Æï³Ë_ÐÞÂÞÕ¶";
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
        virtual ~ Skill264Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 255 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 6 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (28 * skill->GetLevel () + 400);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 100, 102, 104, 106, 108, 110, 112, 113, 115 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (7);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (1.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 28 * skill->GetLevel () + 400, 160 + 20 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (INT (zrand (100) / (100 - 5 * skill->GetT1 ())) * 600));
            skill->GetVictim ()->SetTime (2050 * skill->GetT1 ());
            skill->GetVictim ()->SetDizzy (1);
            skill->GetVictim ()->SetProbability (1.0 * (2 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (100);
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetAmount (0.12 * skill->GetT3 () * skill->GetPlayer ()->GetHp ());
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak2 (1);
            skill->GetVictim ()->SetProbability (1.0 * (120 * int (skill->GetLevel () / 9)));
            skill->GetVictim ()->SetTime (16000 * norm (skill->GetT0 ()));
            skill->GetVictim ()->SetRatio (0.06 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncfatalratio (1);
            return true;
        }
#endif
    };
}
#endif
