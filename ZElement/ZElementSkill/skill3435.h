#ifndef __CPPGEN_GNET_SKILL3435
#define __CPPGEN_GNET_SKILL3435
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill3435:public Skill
    {
      public:
        enum
        { SKILL_ID = 3435 };
          Skill3435 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill3435Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 (skill->GetPlayer ()->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill3435Stub ():SkillStub (3435)
        {
            occupation = 66;
            name = L"»ðÁÔ»¤Ö÷";
            nativename = "»ðÁÔ»¤Ö÷";
            icon = "»ðÁÔ»¤Ö÷.dds";
            maxlevel = 20;
            maxlearn = 8;
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
            talent[0] = 3401;
            talent[1] = 3387;
            talent[2] = 3388;
            talent[3] = 3396;
            talent[4] = 3752;
            talent_size = 5;
            action[0] = "·ÙÏã¹È_»ðá÷»¤Ö÷";
            action[1] = "0";
            action[2] = "0";
            action[3] = "0";
            action[4] = "0";
            action[5] = "0";
            action[6] = "·ÙÏã¹È_»ðá÷»¤Ö÷";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "·ÙÏã¹È_Æï³Ë_»ðá÷»¤Ö÷";
            action[16] = "0";
            action[17] = "0";
            action[18] = "0";
            action[19] = "0";
            action[20] = "0";
            action[21] = "·ÙÏã¹È_Æï³Ë_»ðá÷»¤Ö÷";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "";
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
#endif
        }
        virtual ~ Skill3435Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 3434 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 4 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1000);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 60000 - 60000 * 0.1 * skill->GetT3 () - 60000 * 0.03 * skill->GetT4 ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[8] = { 90, 92, 94, 96, 98, 101, 104, 107 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (8 + skill->GetLevel ());
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
            return _snwprintf (buffer, length, format, (8 + skill->GetLevel ()), 100 + 2 * skill->GetLevel (), 21, (2 + skill->GetLevel ()));

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
            skill->GetVictim ()->SetProbability (1.0 *
                                                 (skill->GetPlayer ()->GetHasbuff (5331) == 0
                                                  && skill->GetPlayer ()->GetLevel () <= (skill->GetPlayer ()->GetVar1 () + 10) ? 600 : 0));
            skill->GetVictim ()->SetTime (10000);
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetValue (2 + skill->GetLevel () + 2 * skill->GetT2 ());
            skill->GetVictim ()->SetSubhide (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetT1 () > 0 && zrand (100) < 33 * skill->GetT1 ()? (-1) : 0));
            skill->GetVictim ()->SetTime (2000 * skill->GetT1 ());
            skill->GetVictim ()->SetWrap (1);
            return true;
        }
        bool BlessMe (Skill * skill) const
        {
            skill->GetVictim ()->SetTime (21000);
            skill->GetVictim ()->SetRatio (1.0 + 0.02 * skill->GetLevel ());
            skill->GetVictim ()->SetAmount (49);
            skill->GetVictim ()->SetValue (55 * skill->GetT2 ());
            skill->GetVictim ()->SetEvilaura (1);
            skill->GetVictim ()->SetRatio (zrand (100) < 40 * skill->GetT0 ()? 3444 : 220);
            skill->GetVictim ()->SetAmount (220);
            skill->GetVictim ()->SetValue (220);
            skill->GetVictim ()->SetClearcooldown (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (40);
        }
#endif
    };
}
#endif
