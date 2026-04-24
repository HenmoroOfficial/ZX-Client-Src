#ifndef __CPPGEN_GNET_SKILL1118
#define __CPPGEN_GNET_SKILL1118
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill1118:public Skill
    {
      public:
        enum
        { SKILL_ID = 1118 };
          Skill1118 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill1118Stub:public SkillStub
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
      Skill1118Stub ():SkillStub (1118)
        {
            occupation = 27;
            name = L"Óü»ğÁ¶»ê";
            nativename = "Óü»ğÁ¶»ê";
            icon = "Óü»ğÁ¶»ê.dds";
            maxlevel = 20;
            maxlearn = 10;
            type = 3;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 2;
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
            talent[0] = 1161;
            talent[1] = 1165;
            talent_size = 2;
            action[0] = "Óü»ğÁ¶»ê";
            action[1] = "Óü»ğÁ¶»ê";
            action[2] = "Óü»ğÁ¶»ê";
            action[3] = "Óü»ğÁ¶»ê";
            action[4] = "Óü»ğÁ¶»ê";
            action[5] = "Óü»ğÁ¶»ê";
            action[6] = "";
            action[7] = "";
            action[8] = "";
            action[9] = "";
            action[10] = "";
            action[11] = "";
            action[12] = "";
            action[13] = "";
            action[14] = "";
            action[15] = "Óü»ğÁ¶»ê";
            action[16] = "Óü»ğÁ¶»ê";
            action[17] = "Óü»ğÁ¶»ê";
            action[18] = "Óü»ğÁ¶»ê";
            action[19] = "Óü»ğÁ¶»ê";
            action[20] = "Óü»ğÁ¶»ê";
            action[21] = "";
            action[22] = "";
            action[23] = "";
            action[24] = "";
            action[25] = "";
            action[26] = "";
            action[27] = "";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 2;
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
        virtual ~ Skill1118Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 1115 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 2 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (120 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 180000 - 18000 * skill->GetT1 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 85, 89, 93, 97, 101, 106, 111, 116, 121, 125 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (6 + 2 * skill->GetLevel ());
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (6 + 2 * skill->GetLevel ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (4 + 3 * skill->GetLevel ());
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               INT (120 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel () * skill->GetLevel ()),
                               6 + 2 * skill->GetLevel (),
                               180 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel () * skill->GetLevel (),
                               90 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel () * skill->GetLevel (),
                               3000 - 100 * skill->GetLevel (), 6 + 3 * skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (6000 + 3000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (3000 - 100 * skill->GetLevel ());
            skill->GetVictim ()->SetValue ((180 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel () * skill->GetLevel ()) *
                                           (1 + 0.15 * skill->GetT0 ()));
            skill->GetVictim ()->SetSubhp (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (100));
            skill->GetVictim ()->SetTime (6000 + 3000 * skill->GetLevel () + 100);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (3000 - 100 * skill->GetLevel ());
            skill->GetVictim ()->SetValue (90 * skill->GetLevel () + 2 * skill->GetLevel () * skill->GetLevel () * skill->GetLevel ());
            skill->GetVictim ()->SetSubhp (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (25);
        }
#endif
    };
}
#endif
