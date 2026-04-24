#ifndef __CPPGEN_GNET_SKILL5270
#define __CPPGEN_GNET_SKILL5270
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill5270:public Skill
    {
      public:
        enum
        { SKILL_ID = 5270 };
          Skill5270 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill5270Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar5 (skill->GetPlayer ()->GetHasbuff (4270));
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill5270Stub ():SkillStub (5270)
        {
            occupation = 168;
            name = L"纵横天下<煞>";
            nativename = "纵横天下<煞>";
            icon = "问号.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 2;
            eventflag = 1;
            spcost = 0;
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
            skill_class = 13;
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
            action[13] = "0";
            action[14] = "0";
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
            action[28] = "0";
            action[29] = "0";
            action[30] = "0";
            rangetype = 2;
            doenchant = false;
            dobless = true;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = 14;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
#endif
        }
        virtual ~ Skill5270Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (5 * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 4877 };
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
                { 400, 500, 500, 700, 1500, 3100, 3900, 6500, 9100, 10800, 20700, 26500, 39600, 52900, 84300, 119800, 229700, 389600, 525000,
 1237600 };
            return array[level - 1];
        }
        int GetDivinityLevel (int level) const
        {
            static int array[20] = { 2, 4, 6, 8, 10, 13, 16, 19, 22, 25, 29, 33, 37, 42, 47, 53, 59, 65, 71, 77 };
            return array[level - 1];
        }
        int GetHpcost (Skill * skill) const
        {
            return (int) (50);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 0;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 500;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[20] = { 120, 122, 124, 126, 128, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 141, 143, 145, 147, 149 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (10);
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (10);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (8);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 10 * skill->GetLevel (), 5 * skill->GetLevel (), 640 + 80 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel (), 4.5 + 0.5 * skill->GetLevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (4500 + 500 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (1 + 0.05 * skill->GetLevel ());
            skill->GetVictim ()->SetAmount (1.0 * (skill->GetPlayer ()->GetMaxhp () - skill->GetPlayer ()->GetHp ()) /
                                            skill->GetPlayer ()->GetMaxhp () * (skill->GetPlayer ()->GetCultivation () ==
                                                                                2 ? skill->GetPlayer ()->GetSkilllevel (5260) * 0.05 : 0));
            skill->GetVictim ()->SetValue (640 + 80 * skill->GetLevel ());
            skill->GetVictim ()->SetCycleskill (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (skill->GetPlayer ()->GetVar5 ()? 10 : (4500 + 500 * skill->GetLevel ()));
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetBuffid (5);
            skill->GetVictim ()->SetValue (5227);
            skill->GetVictim ()->SetDelaycast (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (20);
        }
#endif
    };
}
#endif
