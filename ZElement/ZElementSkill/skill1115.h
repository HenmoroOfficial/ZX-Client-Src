#ifndef __CPPGEN_GNET_SKILL1115
#define __CPPGEN_GNET_SKILL1115
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1115:public Skill
    {
      public:
        enum
        { SKILL_ID = 1115 };
          Skill1115 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1115Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill1115Stub ():SkillStub (1115)
        {
            occupation = 27;
            name = L"Ô¹¹íÐÄÄ§";
            nativename = "Ô¹¹íÐÄÄ§";
            icon = "Ô¹¹íÐÄÄ§.dds";
            maxlevel = 20;
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
            skill_class = 4;
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
            talent[0] = 1162;
            talent[1] = 1168;
            talent[2] = 1170;
            talent_size = 3;
            action[0] = "Ô¹¹íÐÄÄ§";
            action[1] = "Ô¹¹íÐÄÄ§";
            action[2] = "Ô¹¹íÐÄÄ§";
            action[3] = "Ô¹¹íÐÄÄ§";
            action[4] = "Ô¹¹íÐÄÄ§";
            action[5] = "Ô¹¹íÐÄÄ§";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "0";
            action[14] = "0";
            action[15] = "Ô¹¹íÐÄÄ§";
            action[16] = "Ô¹¹íÐÄÄ§";
            action[17] = "Ô¹¹íÐÄÄ§";
            action[18] = "Ô¹¹íÐÄÄ§";
            action[19] = "Ô¹¹íÐÄÄ§";
            action[20] = "Ô¹¹íÐÄÄ§";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "0";
            action[29] = "0";
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
        virtual ~ Skill1115Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1113 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (12 + skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (350 + 30 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 90000 - 9000 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 75, 80, 85, 90, 95, 100, 105, 110, 115, 120 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (10 + skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (8 + skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               350 + 30 * skill->GetLevel (),
                               8 + skill->GetLevel (),
                               2 * skill->GetLevel (), 3 * skill->GetLevel (), 30 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel (), 10);

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
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime ((2000 * skill->GetLevel () + 2000 * skill->GetT2 ()) *
                                          (1 + norm (zrand (100) / (100 - 10 * skill->GetT2 ()))));
            skill->GetVictim ()->SetRatio (0.03 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue ((30 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ()) * (1 + 0.3 * skill->GetT0 ()));
            skill->GetVictim ()->SetXinmo (1);
            skill->GetVictim ()->SetProbability (1.0 * (120 * int (skill->GetLevel () / 10)));
            skill->GetVictim ()->SetTime ((2000 * skill->GetLevel () + 2000 * skill->GetT2 ()) *
                                          (1 + norm (zrand (100) / (100 - 10 * skill->GetT2 ()))));
            skill->GetVictim ()->SetRatio (0.1);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncfatalratio (1);
            skill->GetVictim ()->SetProbability (1.0 * (120 * int (skill->GetLevel () / 10) * int (zrand (100) / (100 - 8 * skill->GetT0 ()))));
            skill->GetVictim ()->SetTime ((2000 * skill->GetLevel () + 2000 * skill->GetT2 ()) *
                                          (1 + norm (zrand (100) / (100 - 10 * skill->GetT2 ()))));
            skill->GetVictim ()->SetRatio (0.04 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetIncfatalratio (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime ((2000 * skill->GetLevel () + 2000 * skill->GetT2 ()) *
                                          (1 + norm (zrand (100) / (100 - 10 * skill->GetT2 ()))));
            skill->GetVictim ()->SetRatio (0.3 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetIncfatalhurt (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100 * skill->GetT0 ()));
            skill->GetVictim ()->SetClearweak (1);
            return true;
        }
#endif
    };
}
#endif
