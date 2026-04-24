#ifndef __CPPGEN_GNET_SKILL3027
#define __CPPGEN_GNET_SKILL3027
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3027:public Skill
    {
      public:
        enum
        { SKILL_ID = 3027 };
          Skill3027 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3027Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
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
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3027Stub ():SkillStub (3027)
        {
            occupation = 167;
            name = L"´óèó°ãÈô<Ðþ>";
            nativename = "´óèó°ãÈô<Ðþ>";
            icon = "ÏÉ¡¤´óèó°ãÈô.dds";
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
            clearmask = 1;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
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
            talent[0] = 693;
            talent[1] = 700;
            talent[2] = 716;
            talent_size = 3;
            action[0] = "ÌìÒôËÂ_´ó·²°ãÈô";
            action[1] = "ÌìÒôËÂ_´ó·²°ãÈô";
            action[2] = "ÌìÒôËÂ_´ó·²°ãÈô";
            action[3] = "ÌìÒôËÂ_´ó·²°ãÈô";
            action[4] = "ÌìÒôËÂ_´ó·²°ãÈô";
            action[5] = "ÌìÒôËÂ_´ó·²°ãÈô";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÌìÒôËÂ_Æï³Ë_´ó·²°ãÈô";
            action[16] = "ÌìÒôËÂ_Æï³Ë_´ó·²°ãÈô";
            action[17] = "ÌìÒôËÂ_Æï³Ë_´ó·²°ãÈô";
            action[18] = "ÌìÒôËÂ_Æï³Ë_´ó·²°ãÈô";
            action[19] = "ÌìÒôËÂ_Æï³Ë_´ó·²°ãÈô";
            action[20] = "ÌìÒôËÂ_Æï³Ë_´ó·²°ãÈô";
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
        virtual ~ Skill3027Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (180 + 26 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 789 };
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
            return (float) (810 + 15 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel ());
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
                               810 + 15 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel (),
                               180 + 26 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               180, 20, 2, 3 * skill->GetLevel (), 2 * skill->GetLevel (), skill->GetLevel ());

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
            skill->GetVictim ()->SetTime (20000 + 2000 * skill->GetT2 () + 100);
            skill->GetVictim ()->SetRatio (0.02 * skill->GetLevel () + 0.02 * skill->GetT0 () - 0.002);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (0.03 * skill->GetLevel () * skill->GetPlayer ()->GetMaxhp () * 10 * (1 + 0.2 * skill->GetT1 ()) *
                                           (1 + 0.1 * skill->GetT2 ()));
            skill->GetVictim ()->SetBanruo (1);
            skill->GetVictim ()->SetTime (2000 + 100 + 2000 * skill->GetT2 ());
            skill->GetVictim ()->SetInvincible (1);
            skill->GetVictim ()->SetTime (20000 + 2000 * skill->GetT2 () + 100);
            skill->GetVictim ()->SetRatio (0.01 * skill->GetLevel () + 0.003);
            skill->GetVictim ()->SetInccritrate (1);
            return true;
        }
#endif
    };
}
#endif
