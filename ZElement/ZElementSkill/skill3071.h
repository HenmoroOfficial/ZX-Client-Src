#ifndef __CPPGEN_GNET_SKILL3071
#define __CPPGEN_GNET_SKILL3071
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3071:public Skill
    {
      public:
        enum
        { SKILL_ID = 3071 };
          Skill3071 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3071Stub:public SkillStub
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
                return 1200;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes1 ());
                skill->SetCrit (0.02 * skill->GetT0 ());
                skill->SetRatio (1 + 0.07 * skill->GetT1 ());
                skill->SetCrithurt (0.5 * skill->GetT3 ());
                skill->SetPlus (100 * skill->GetLevel () * (1 + 0.3 * skill->GetT3 ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3071Stub ():SkillStub (3071)
        {
            occupation = 168;
            name = L"毁天灭地<煞>";
            nativename = "毁天灭地<煞>";
            icon = "魔·毁天灭地.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 1;
            eventflag = 0;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 0;
            clearmask = 2;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
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
            talent[0] = 628;
            talent[1] = 633;
            talent[2] = 645;
            talent[3] = 668;
            talent_size = 4;
            action[0] = "鬼王宗_毁天灭地";
            action[1] = "鬼王宗_毁天灭地";
            action[2] = "鬼王宗_毁天灭地";
            action[3] = "鬼王宗_毁天灭地";
            action[4] = "鬼王宗_毁天灭地";
            action[5] = "鬼王宗_毁天灭地";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "鬼王宗_骑乘_毁天灭地";
            action[16] = "鬼王宗_骑乘_毁天灭地";
            action[17] = "鬼王宗_骑乘_毁天灭地";
            action[18] = "鬼王宗_骑乘_毁天灭地";
            action[19] = "鬼王宗_骑乘_毁天灭地";
            action[20] = "鬼王宗_骑乘_毁天灭地";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
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
        virtual ~ Skill3071Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 777 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 200, 500, 500, 600, 900, 1900, 3200, 4500, 8600, 10800, 20700, 32200, 51000, 65200, 84800, 159900, 260100, 427500, 743600,
 1450600 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 1, 3, 5, 7, 9, 11, 14, 17, 21, 25, 29, 34, 39, 44, 49, 55, 61, 67, 73, 79 };
            return array[level - 1];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (950 + 15 * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 20000 - 2000 * skill->GetT1 ();
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (13);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (13);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (11);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (950 + 15 * skill->GetLevel ()),
                               200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               20,
                               INT (6 + INT (skill->GetLevel () / 4.9) * 2),
                               13, 100 * skill->GetLevel (), 33, 120 * skill->GetLevel (), 10 + 3 * skill->GetLevel (), skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (33 + 15 * skill->GetT0 ()));
            skill->GetVictim ()->SetTime (20000 + 100);
            skill->GetVictim ()->SetAssault (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (2000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (120 * skill->GetLevel () * (1 + 0.8 * skill->GetT2 ()) *
                                            (1 + INT (zrand (100) / (100 - 6 * skill->GetT2 ()))));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetHpleak5 (1);
            skill->GetVictim ()->SetProbability (1.0 * (5 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (6000 + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (100);
            skill->GetVictim ()->SetValue (skill->GetPlayer ()->GetVar1 () * skill->GetLevel ());
            skill->GetVictim ()->SetSubhp (1);
            skill->GetVictim ()->SetProbability (1.0 * (10 + 3 * skill->GetLevel ()));
            skill->GetVictim ()->SetPullback (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (6 + INT (skill->GetLevel () / 4.9) * 2);
        }
        float GetDropRate (Skill * skill) const
        {
            return (float) (0.5);
        }
#endif
    };
}
#endif
