#ifndef __CPPGEN_GNET_SKILL3082
#define __CPPGEN_GNET_SKILL3082
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3082:public Skill
    {
      public:
        enum
        { SKILL_ID = 3082 };
          Skill3082 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3082Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3082Stub ():SkillStub (3082)
        {
            occupation = 168;
            name = L"邪神傲世<煞>";
            nativename = "邪神傲世<煞>";
            icon = "魔·邪神傲世.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 2;
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
            talent[0] = 1168;
            talent[1] = 1169;
            talent[2] = 1170;
            talent_size = 3;
            action[0] = "邪神傲世";
            action[1] = "邪神傲世";
            action[2] = "邪神傲世";
            action[3] = "邪神傲世";
            action[4] = "邪神傲世";
            action[5] = "邪神傲世";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "0";
            action[14] = "0";
            action[15] = "邪神傲世";
            action[16] = "邪神傲世";
            action[17] = "邪神傲世";
            action[18] = "邪神傲世";
            action[19] = "邪神傲世";
            action[20] = "邪神傲世";
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
            rangetype = 5;
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
        virtual ~ Skill3082Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (180 + 26 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1132 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 5 };
            return array[index];
        }
        int GetDivinityExp (int level) const
        {
            static int array[20] =
                { 400, 500, 500, 700, 1500, 2400, 3500, 5600, 8500, 8600, 19000, 26500, 49100, 57600, 85800, 140300, 241000, 410600, 614500,
 1344300 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 12, 15, 18, 21, 24, 28, 33, 38, 43, 48, 54, 60, 66, 72, 78 };
            return array[level - 1];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000 - 18000 * skill->GetT0 () - 4000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               0,
                               180 + 26 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               180 - 4 * skill->GetLevel (),
                               12,
                               10 + skill->GetLevel (),
                               10 + skill->GetLevel (), 10 + skill->GetLevel (), 10 + skill->GetLevel (), 3 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime ((12000 + 2000 * skill->GetT2 ()) * (1 + norm (zrand (100) / (100 - 10 * skill->GetT2 ()))) + 100);
            skill->GetVictim ()->SetRatio (0.1 + 0.01 * skill->GetLevel () + 0.02 * skill->GetT1 ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetAoshi (1);
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime ((12000 + 2000 * skill->GetT2 ()) * (1 + norm (zrand (100) / (100 - 10 * skill->GetT2 ()))));
            skill->GetVictim ()->SetRatio (0.03 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetInccrithurt (1);
            return true;
        }
#endif
    };
}
#endif
