#ifndef __CPPGEN_GNET_SKILL4847
#define __CPPGEN_GNET_SKILL4847
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4847:public Skill
    {
      public:
        enum
        { SKILL_ID = 4847 };
          Skill4847 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4847Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 650;
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
                return 550;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetSkillaccu (50 * skill->GetT1 ());
                skill->GetPlayer ()->
                    SetVar1 ((((skill->GetPlayer ()->GetCultivation () == 4)
                               && (skill->GetPlayer ()->GetSkilllevel (5262) >
                                   0)) ? ((skill->GetPlayer ()->GetRes2 () + skill->GetPlayer ()->GetSkilllevel (5262) * 8) >
                                          600 ? (skill->GetPlayer ()->GetRes2 () +
                                                 skill->GetPlayer ()->GetSkilllevel (5262) * 8) : 600) : (skill->GetPlayer ()->GetLevel () * 3 +
                                                                                                          10 * skill->GetLevel ())));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4847Stub ():SkillStub (4847)
        {
            occupation = 108;
            name = L"Ç£ÖÆÊõ";
            nativename = "Ç£ÖÆÊõ";
            icon = "Ç£ÖÆÕ½Êõ.dds";
            maxlevel = 10;
            maxlearn = 9;
            type = 1;
            eventflag = 0;
            spcost = 1;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
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
            talent[0] = 5204;
            talent[1] = 5207;
            talent_size = 2;
            action[0] = "Ó¢ÕÐ_¹´»êÇ¹";
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
            action[14] = "Ó¢ÕÐ_¹´»êÇ¹";
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
            action[29] = "Ó¢ÕÐ_¹´»êÇ¹";
            action[30] = "0";
            rangetype = 0;
            doenchant = true;
            dobless = false;
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
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill4847Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 4844 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 3 };
            return array[index];
        }
        int GetHpcost (Skill * skill) const
        {
            return (int) (10);
        }
        int GetInkcost (Skill * skill) const
        {
            return (int) (0);
        }
        float GetEffectdistance (Skill * skill) const
        {
            return (float) (25);
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (0);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1200;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 30000;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[9] = { 30, 32, 34, 36, 38, 40, 42, 44, 46 };
            return array[level - 1];
        }
        float GetAngle (Skill * skill) const
        {
            return (float) (1 - 0.0111111 * (20));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (17);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 10 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetType () == 0 ? 120 : 0));
            skill->GetVictim ()->SetPullback (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetVar1 ()));
            skill->GetVictim ()->SetTime (4000);
            skill->GetVictim ()->SetRatio (0.5);
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetWeak (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetAmount (skill->GetT0 () > 0 ? skill->GetT0 () : 0);
            skill->GetVictim ()->SetClearbuff (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) ((skill->GetPlayer ()->GetCultivation () == 4) ? skill->GetPlayer ()->GetSkilllevel (5262) : 1);
        }
#endif
    };
}
#endif
