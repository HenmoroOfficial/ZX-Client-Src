#ifndef __CPPGEN_GNET_SKILL3058
#define __CPPGEN_GNET_SKILL3058
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3058:public Skill
    {
      public:
        enum
        { SKILL_ID = 3058 };
          Skill3058 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3058Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRes1 ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3058Stub ():SkillStub (3058)
        {
            occupation = 168;
            name = L"¶áÆÇ";
            nativename = "¶áÆÇ";
            icon = "Ð°¶ñ½ûïÀ.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 3;
            eventflag = 0;
            spcost = 1;
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
            skill_class = -2;
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
            preskillnum = 0;
            action[0] = "Ä§_ÖÂËÀ´ò»÷";
            action[1] = "Ä§_ÖÂËÀ´ò»÷";
            action[2] = "Ä§_ÖÂËÀ´ò»÷";
            action[3] = "Ä§_ÖÂËÀ´ò»÷";
            action[4] = "Ä§_ÖÂËÀ´ò»÷";
            action[5] = "Ä§_ÖÂËÀ´ò»÷";
            action[6] = "Ä§_ÖÂËÀ´ò»÷";
            action[7] = "Ä§_ÖÂËÀ´ò»÷";
            action[8] = "Ä§_ÖÂËÀ´ò»÷";
            action[9] = "Ä§_ÖÂËÀ´ò»÷";
            action[10] = "Ä§_ÖÂËÀ´ò»÷";
            action[11] = "Ä§_ÖÂËÀ´ò»÷";
            action[12] = "Ä§_ÖÂËÀ´ò»÷";
            action[13] = "Ä§_ÖÂËÀ´ò»÷";
            action[14] = "Ä§_ÖÂËÀ´ò»÷";
            action[15] = "Æï³Ë_Ä§_ÖÂËÀ´ò»÷";
            action[16] = "Æï³Ë_Ä§_ÖÂËÀ´ò»÷";
            action[17] = "Æï³Ë_Ä§_ÖÂËÀ´ò»÷";
            action[18] = "Æï³Ë_Ä§_ÖÂËÀ´ò»÷";
            action[19] = "Æï³Ë_Ä§_ÖÂËÀ´ò»÷";
            action[20] = "Æï³Ë_Ä§_ÖÂËÀ´ò»÷";
            action[21] = "Æï³Ë_Ä§_ÖÂËÀ´ò»÷";
            action[22] = "Æï³Ë_Ä§_ÖÂËÀ´ò»÷";
            action[23] = "Æï³Ë_Ä§_ÖÂËÀ´ò»÷";
            action[24] = "Æï³Ë_Ä§_ÖÂËÀ´ò»÷";
            action[25] = "Æï³Ë_Ä§_ÖÂËÀ´ò»÷";
            action[26] = "Æï³Ë_Ä§_ÖÂËÀ´ò»÷";
            action[27] = "Æï³Ë_Ä§_ÖÂËÀ´ò»÷";
            action[28] = "Æï³Ë_Ä§_ÖÂËÀ´ò»÷";
            action[29] = "Æï³Ë_Ä§_ÖÂËÀ´ò»÷";
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
        virtual ~ Skill3058Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel ());
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
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (15);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 2000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 120000 - 4000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (18);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (18);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (16);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               190 + 50 * skill->GetLevel () + 4 * skill->GetLevel () * skill->GetLevel (),
                               120 - 4 * skill->GetLevel (),
                               INT (3 + skill->GetLevel () / 4.9), 20 + 3 * skill->GetLevel (), 1 + 0.2 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (20 + 3 * skill->GetLevel () + norm (skill->GetLevel () / 19.9) * 10));
            skill->GetVictim ()->SetPullback (1);
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (zrand (100) + 80 >
                                                  100 ? (900 < skill->GetPlayer ()->GetVar1 ()? skill->GetPlayer ()->GetVar1 () : 900) : 0));
            skill->GetVictim ()->SetTime (skill->GetPlayer ()->GetIsignite ()? 5050 + 200 * skill->GetLevel () : 1050 + 200 * skill->GetLevel ());
            skill->GetVictim ()->SetDizzy (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (3 + INT (skill->GetLevel () / 4.9) + norm (skill->GetLevel () / 19.9) * 2);
        }
#endif
    };
}
#endif
