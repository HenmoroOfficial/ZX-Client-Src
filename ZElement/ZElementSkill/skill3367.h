#ifndef __CPPGEN_GNET_SKILL3367
#define __CPPGEN_GNET_SKILL3367
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3367:public Skill
    {
      public:
        enum
        { SKILL_ID = 3367 };
          Skill3367 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3367Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 0;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3367Stub ():SkillStub (3367)
        {
            occupation = 64;
            name = L"Ô­Áé";
            nativename = "Ô­Áé";
            icon = "Ô­Áé.dds";
            maxlevel = 20;
            maxlearn = 6;
            type = 2;
            eventflag = 0;
            spcost = 1;
            timetype = 1;
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
            skill_class = 9;
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
            talent[0] = 3390;
            talent[1] = 3394;
            talent[2] = 3403;
            talent_size = 3;
            action[0] = "0";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "0";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "0";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "0";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "";
            rangetype = 5;
            doenchant = true;
            dobless = true;
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
#endif
        }
        virtual ~ Skill3367Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3364 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 1 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (45);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 40000 - 10000 * skill->GetT0 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[6] = { 16, 18, 21, 24, 28, 32 };
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
                               45,
                               (0.8 + 0.2 * skill->GetLevel ()),
                               (35 - 5 * skill->GetLevel ()), (45 - 5 * skill->GetLevel ()), (55 - 5 * skill->GetLevel ()));

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
            skill->GetVictim ()->SetValue (0.8 + 0.2 * skill->GetLevel ());
            skill->GetVictim ()->SetYuanling (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (1000 * (35 - 5 * skill->GetLevel ()) + 50);
            skill->GetVictim ()->SetRatio (1 + 10 * skill->GetT1 () + 100 * skill->GetT2 ());
            skill->GetVictim ()->SetBuffid (7);
            skill->GetVictim ()->SetValue (3596);
            skill->GetVictim ()->SetDelaycast (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetHasbuff (4221) == 1 ? 0 : 100));
            skill->GetVictim ()->SetRatio (5447);
            skill->GetVictim ()->SetAmount (4411);
            skill->GetVictim ()->SetValue (5447);
            skill->GetVictim ()->SetDispel (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetClearwrap (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetClearslow (1);
            return true;
        }
#endif
    };
}
#endif
