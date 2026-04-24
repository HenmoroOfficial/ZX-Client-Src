#ifndef __CPPGEN_GNET_SKILL5271
#define __CPPGEN_GNET_SKILL5271
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill5271:public Skill
    {
      public:
        enum
        { SKILL_ID = 5271 };
          Skill5271 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill5271Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 500;
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
                return 500;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetRage ());
                skill->GetPlayer ()->SetRage (0);
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill5271Stub ():SkillStub (5271)
        {
            occupation = 168;
            name = L"°ÔÎÞË«<É·>";
            nativename = "°ÔÎÞË«<É·>";
            icon = "ÎÊºÅ.dds";
            maxlevel = 20;
            maxlearn = 20;
            type = 2;
            eventflag = 1;
            spcost = 0;
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
            talent[0] = 5186;
            talent_size = 1;
            action[0] = "Ó¢ÕÐ_·´ÉË";
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
            action[14] = "Ó¢ÕÐ_·´ÉË";
            action[15] = "Ó¢ÕÐ_·´ÉË";
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
            action[29] = "Ó¢ÕÐ_·´ÉË";
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
        virtual ~ Skill5271Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (170 + 42 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel ());
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 4878 };
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
        int GetHpcost (Skill * skill) const
        {
            return (int) (0);
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
            return 60000;
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
                               170 + 42 * skill->GetLevel () + 3 * skill->GetLevel () * skill->GetLevel (),
                               3, 5, 50 + skill->GetLevel (), 20 * skill->GetLevel (), 60);

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT0 () * 25));
            skill->GetVictim ()->SetTime (skill->GetT0 () > 0 ? (5000 + 1000 * skill->GetPlayer ()->GetSkilllevel (4878)) : 0);
            skill->GetVictim ()->SetRejectdebuff (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (5000 + 1000 * skill->GetPlayer ()->GetSkilllevel (4878));
            skill->GetVictim ()->SetRatio (3);
            skill->GetVictim ()->
                SetAmount ((skill->GetPlayer ()->GetMaxatk () * 5 + skill->GetPlayer ()->GetMaxatk () * 0.2 * skill->GetLevel () +
                            skill->GetPlayer ()->GetVar1 () * 0.5 +
                            skill->GetPlayer ()->GetVar1 () * 0.01 * skill->GetLevel ()) * (skill->GetPlayer ()->GetDmgreduce () <
                                                                                            999 ? (1.0 /
                                                                                                   (1000 -
                                                                                                    skill->GetPlayer ()->GetDmgreduce ())) : 1.0) *
                           1000);
            skill->GetVictim ()->SetValue (100);
            skill->GetVictim ()->SetRetort (1);
            return true;
        }
#endif
    };
}
#endif
