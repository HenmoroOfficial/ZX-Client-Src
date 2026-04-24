#ifndef __CPPGEN_GNET_SKILL3144
#define __CPPGEN_GNET_SKILL3144
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3144:public Skill
    {
      public:
        enum
        { SKILL_ID = 3144 };
          Skill3144 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3144Stub:public SkillStub
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
      Skill3144Stub ():SkillStub (3144)
        {
            occupation = 169;
            name = L"影舞·极<禅>";
            nativename = "影舞·极<禅>";
            icon = "佛·影舞·极.dds";
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
            clearmask = 4;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 7;
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
            talent[0] = 2566;
            talent[1] = 2562;
            talent[2] = 2564;
            talent[3] = 2567;
            talent[4] = 2568;
            talent_size = 5;
            action[0] = "怀光_死神之影";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "怀光_死神之影";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "怀光_骑乘_死神之影";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "怀光_骑乘_死神之影";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
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
            skill_limit = 1536;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill3144Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 2453 };
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
                { 200, 500, 500, 600, 900, 2400, 3500, 5600, 8000, 9100, 21700, 36400, 51500, 77500, 79800, 179300, 290600, 437200, 872700, 1620700 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 1, 3, 5, 7, 9, 12, 15, 18, 22, 26, 30, 35, 40, 45, 50, 56, 62, 68, 74, 80 };
            return array[level - 1];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1500 + 4 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000;
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
                               1500 + 4 * skill->GetLevel () * skill->GetLevel (),
                               200 + 34 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               15 + skill->GetLevel (), 180, 50, 50, 5 + 0.5 * skill->GetLevel (), 20 + 3 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (15050 + 1000 * skill->GetLevel () + 5000 * skill->GetT0 ());
            skill->GetVictim ()->SetRatio (3);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetAmount (1 + INT (skill->GetLevel () / 19.9));
            skill->GetVictim ()->SetValue (34670);
            skill->GetVictim ()->SetSummonclone (1);
            skill->GetVictim ()->SetProbability (1.0 * (20 + 3 * skill->GetLevel ()));
            skill->GetVictim ()->SetTime (5050 + 500 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (2);
            skill->GetVictim ()->SetValue (2);
            skill->GetVictim ()->SetAddspeed (1);
            return true;
        }
#endif
    };
}
#endif
