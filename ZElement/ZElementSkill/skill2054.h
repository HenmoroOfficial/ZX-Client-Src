#ifndef __CPPGEN_GNET_SKILL2054
#define __CPPGEN_GNET_SKILL2054
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2054:public Skill
    {
      public:
        enum
        { SKILL_ID = 2054 };
          Skill2054 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2054Stub:public SkillStub
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
                skill->GetPlayer ()->SetVar1 ((0.5 + 0.05 * skill->GetLevel ()) * skill->GetPlayer ()->GetMp ());
                skill->GetPlayer ()->SetMp ((0.5 - 0.05 * skill->GetLevel ()) * skill->GetPlayer ()->GetMp ());
                skill->SetRatio (1 + 0.04 * (skill->GetT0 () + skill->GetT1 () + skill->GetT2 ()));
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2054Stub ():SkillStub (2054)
        {
            occupation = 149;
            name = L"ÆÆÄ§½£ÆøII";
            nativename = "ÆÆÄ§½£ÆøII";
            icon = "ÆÆÄ§½£ÆøII.dds";
            maxlevel = 10;
            maxlearn = 10;
            type = 3;
            eventflag = 1;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 1;
            allowform = 0;
            credittype = 10;
            clearmask = 130;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 2;
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
            talent[0] = 1563;
            talent[1] = 1564;
            talent[2] = 2055;
            talent_size = 3;
            action[0] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[1] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[2] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[3] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[4] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[5] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[6] = "ÇàÔÆÃÅ_ÆÆÄ§½£Æø";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[16] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[17] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[18] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[19] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[20] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[21] = "ÇàÔÆÃÅ_Æï³Ë_ÆÆÄ§½£Æø";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
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
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2054Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 228 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 9 };
            return array[index];
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
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return (380 - 20 * skill->GetLevel ()) * 1000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (20);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (15);
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               50 + 5 * skill->GetLevel (), 50 + 5 * skill->GetLevel (), 10 + 2 * skill->GetLevel (), 380 - 20 * skill->GetLevel ());

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
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetVar1 ());
            skill->GetVictim ()->SetMpdisperse (1);
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (10000 + 2000 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (skill->GetPlayer ()->GetVar1 () / 5 * (5 + skill->GetLevel ()));
            skill->GetVictim ()->SetValue (0);
            skill->GetVictim ()->SetMpleak (1);
            return true;
        }
#endif
    };
}
#endif
