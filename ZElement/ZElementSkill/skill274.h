#ifndef __CPPGEN_GNET_SKILL274
#define __CPPGEN_GNET_SKILL274
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill274:public Skill
    {
      public:
        enum
        { SKILL_ID = 274 };
          Skill274 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill274Stub:public SkillStub
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
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (1 + 0.07 * skill->GetT2 ());
                skill->SetPlus ((224 + 20 * skill->GetLevel ()) * (1 + INT (zrand (99 + 7 * skill->GetT3 ()) / 100)));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill274Stub ():SkillStub (274)
        {
            occupation = 11;
            name = L"°ãÈô¾­";
            nativename = "°ãÈô¾­";
            icon = "°ãÈô¾­.dds";
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
            skill_class = 3;
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
            talent[0] = 692;
            talent[1] = 698;
            talent[2] = 702;
            talent[3] = 708;
            talent_size = 4;
            action[0] = "ÌìÒôËÂ_°ãÈô¾­";
            action[1] = "ÌìÒôËÂ_°ãÈô¾­";
            action[2] = "ÌìÒôËÂ_°ãÈô¾­";
            action[3] = "ÌìÒôËÂ_°ãÈô¾­";
            action[4] = "ÌìÒôËÂ_°ãÈô¾­";
            action[5] = "ÌìÒôËÂ_°ãÈô¾­";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_°ãÈô¾­";
            action[16] = "ÌìÒôËÂ_Æï³Ë_°ãÈô¾­";
            action[17] = "ÌìÒôËÂ_Æï³Ë_°ãÈô¾­";
            action[18] = "ÌìÒôËÂ_Æï³Ë_°ãÈô¾­";
            action[19] = "ÌìÒôËÂ_Æï³Ë_°ãÈô¾­";
            action[20] = "ÌìÒôËÂ_Æï³Ë_°ãÈô¾­";
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
        virtual ~ Skill274Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 0 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 24 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (19);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (20);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 4000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 45, 47, 49, 51, 53, 55, 57, 59, 61 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (16);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (10.7);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               20,
                               224 + 20 * skill->GetLevel (),
                               4 + 4 * skill->GetLevel (), INT (31 + 2.2 * skill->GetLevel ()), 8 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime ((4000 + 4000 * skill->GetLevel ()) * (1 + 0.3 * skill->GetT0 ()) + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (2.2 * skill->GetLevel () + 31 + 2 * skill->GetLevel () * skill->GetT0 ());
            skill->GetVictim ()->SetSubantisleep (1);
            skill->GetVictim ()->SetProbability (1.0 * (6 * skill->GetT1 ()));
            skill->GetVictim ()->SetTime (4050 * skill->GetT1 ());
            skill->GetVictim ()->SetRatio (0.03 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetDecdefence (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetValue (8 * skill->GetLevel () * (1 + skill->GetT3 ()));
            skill->GetVictim ()->SetJuqi (1);
            return true;
        }
#endif
    };
}
#endif
