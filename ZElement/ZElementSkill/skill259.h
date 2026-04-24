#ifndef __CPPGEN_GNET_SKILL259
#define __CPPGEN_GNET_SKILL259
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill259:public Skill
    {
      public:
        enum
        { SKILL_ID = 259 };
          Skill259 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill259Stub:public SkillStub
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
      Skill259Stub ():SkillStub (259)
        {
            occupation = 2;
            name = L"指天骂地";
            nativename = "指天骂地";
            icon = "指天骂地.dds";
            maxlevel = 8;
            maxlearn = 6;
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
            talent[0] = 643;
            talent[1] = 645;
            talent_size = 2;
            action[0] = "鬼王宗_群体激怒";
            action[1] = "鬼王宗_群体激怒";
            action[2] = "鬼王宗_群体激怒";
            action[3] = "鬼王宗_群体激怒";
            action[4] = "鬼王宗_群体激怒";
            action[5] = "鬼王宗_群体激怒";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "鬼王宗_骑乘_群体激怒";
            action[16] = "鬼王宗_骑乘_群体激怒";
            action[17] = "鬼王宗_骑乘_群体激怒";
            action[18] = "鬼王宗_骑乘_群体激怒";
            action[19] = "鬼王宗_骑乘_群体激怒";
            action[20] = "鬼王宗_骑乘_群体激怒";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "技能攻击1";
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
        virtual ~ Skill259Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 257 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (11 + 2 * skill->GetLevel ());
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1.2 * skill->GetLevel () * skill->GetLevel () + 30 * skill->GetLevel () + 224);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 165000 - 15000 * skill->GetLevel () - 10000 * skill->GetT0 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[6] = { 65, 69, 73, 77, 81, 83 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (4 + 3 * skill->GetLevel ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (10 + 2 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (8 + 2 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (1.2 * skill->GetLevel () * skill->GetLevel () + 30 * skill->GetLevel () + 224),
                               4 + 3 * skill->GetLevel (),
                               52 + 6 * skill->GetLevel (), 165 - 15 * skill->GetLevel (), 8 + 2 * skill->GetLevel (), 60 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (52 + 6 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (12000);
            skill->GetVictim ()->SetAssault (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (2000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (60 * skill->GetLevel () * (1 + 0.8 * skill->GetT1 ()) *
                                            (1 + INT (zrand (100) / (100 - 6 * skill->GetLevel ()))));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak5 (1);
            skill->GetVictim ()->SetTime (6050 * skill->GetT0 ());
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetValue (15 * skill->GetT0 ());
            skill->GetVictim ()->SetSubantidizzy (1);
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
