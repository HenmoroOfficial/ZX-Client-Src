#ifndef __CPPGEN_GNET_SKILL4877
#define __CPPGEN_GNET_SKILL4877
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill4877:public Skill
    {
      public:
        enum
        { SKILL_ID = 4877 };
          Skill4877 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill4877Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar4 (skill->GetPlayer ()->GetSkilllevel (4864));
                skill->GetPlayer ()->SetVar3 (skill->GetPlayer ()->GetSkilllevel (4855));
                skill->GetPlayer ()->SetVar2 (skill->GetPlayer ()->GetSkilllevel (4843));
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetMaxatk ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill4877Stub ():SkillStub (4877)
        {
            occupation = 112;
            name = L"纵横天下";
            nativename = "纵横天下";
            icon = "纵横天下.dds";
            maxlevel = 6;
            maxlearn = 5;
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
            talent[0] = 5203;
            talent_size = 1;
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
            doenchant = true;
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
        virtual ~ Skill4877Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 0 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 24 };
            return array[index];
        }
        int GetHpcost (Skill * skill) const
        {
            return (int) (50);
        }
        int GetInkcost (Skill * skill) const
        {
            return (int) (0);
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
            return 0;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 500;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[5] = { 135, 136, 137, 138, 139 };
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
            return _snwprintf (buffer, length, format, 640 + 80 * skill->GetLevel ());

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
            skill->GetVictim ()->SetProbability (1.0 * (((skill->GetT0 () > 0) && (skill->GetPlayer ()->GetVar2 () > 0)) ? 600 : 0));
            skill->GetVictim ()->SetTime (4000);
            skill->GetVictim ()->SetRatio (0.25 + 0.05 * skill->GetPlayer ()->GetVar2 ());
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetSlow (1);
            skill->GetVictim ()->SetProbability (1.0 * (((skill->GetT0 () > 1) && (skill->GetPlayer ()->GetVar3 () > 0)) ? 120 : 0));
            skill->GetVictim ()->SetTime (6000);
            skill->GetVictim ()->SetRatio (0.05 * skill->GetPlayer ()->GetVar3 ());
            skill->GetVictim ()->SetBuffid (0);
            skill->GetVictim ()->SetDecdefence (1);
            skill->GetVictim ()->SetProbability (1.0 * (((skill->GetT0 () > 2) && (skill->GetPlayer ()->GetVar4 () > 0)) ? 120 : 0));
            skill->GetVictim ()->SetValue ((skill->GetPlayer ()->GetMaxhp () - skill->GetPlayer ()->GetHp ()) * 0.01 *
                                           skill->GetPlayer ()->GetVar4 () <
                                           skill->GetPlayer ()->GetVar1 ()? (skill->GetPlayer ()->GetMaxhp () -
                                                                             skill->GetPlayer ()->GetHp ()) * 0.01 *
                                           skill->GetPlayer ()->GetVar4 () : skill->GetPlayer ()->GetVar1 ());
            skill->GetVictim ()->SetDirecthurt (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetRatio (1);
            skill->GetVictim ()->SetAmount (0);
            skill->GetVictim ()->SetValue (640 + 80 * skill->GetLevel ());
            skill->GetVictim ()->SetCycleskill (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (skill->GetPlayer ()->GetVar5 ()? 10 : 10000);
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
